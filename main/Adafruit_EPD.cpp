#include "Adafruit_EPD.h"
#include <stdlib.h>

bool Adafruit_EPD::_isInTransaction = false;

#define BUSY_WAIT 500

// clang-format off

const uint8_t acep_default_init_code[] {
  0xFF, 10, // wait a lil bit
  ACEP_PANEL_SETTING, 2, 0xEF, 0x08, // LUT from OTP
  ACEP_POWER_SETTING, 4, 0x37, 0x00, 0x23, 0x23, // 0x05&0x05?
  ACEP_POWER_OFF_SEQUENCE, 1, 0x00,
  ACEP_BOOSTER_SOFT_START, 3, 0xC7, 0xC7, 0x1D,
  ACEP_PLL, 1, 0x3C,
  ACEP_TSE, 1, 0x00,
  ACEP_CDI, 1, 0x37,
  ACEP_TCON, 1, 0x22,
  ACEP_RESOLUTION, 4, 0x02, 0x58, 0x01, 0xC0,
  ACEP_PWS, 1, 0xAA,
  0xFF, 100, // 100 ms delay
  ACEP_CDI, 1, 0x37,
  0xFE};


// constructor for hardware SPI - we indicate DataCommand, ChipSelect, Reset
/**************************************************************************/
/*!
    @brief constructor if using on-chip RAM and hardware SPI
    @param width the width of the display in pixels
    @param height the height of the display in pixels
    @param DC the data/command pin to use
    @param RST the reset pin to use
    @param CS the chip select pin to use
    @param SRCS the SRAM chip select pin to use
    @param BUSY the busy pin to use
    @param spi the SPI bus to use
*/
/**************************************************************************/
Adafruit_EPD::Adafruit_EPD(int _width, int _height, int16_t DC, int16_t RST,
                           int16_t CS, int16_t BUSY,
                           SPIClass *spi)
{
  height = _height;
  width = _width;
  if ((height % 8) != 0) {
    height += 8 - (height % 8);
  }
  buffer1_size = (uint16_t)width * (uint16_t)height / 2 / 2;
  buffer2_size = (uint16_t)width * (uint16_t)height / 2 / 4;
  buffer3_size = (uint16_t)width * (uint16_t)height / 2 / 4;


  buffer1 = (uint8_t *)malloc(buffer1_size);
  buffer2 = (uint8_t *)malloc(buffer2_size);
	buffer3 = (uint8_t *)malloc(buffer3_size);

  singleByteTxns = true;
  _cs_pin = CS;
  _reset_pin = RST;
  _dc_pin = DC;
  _busy_pin = BUSY;
  Serial.printf("Display Init:\nBuf1 %p Len %i Buf2 %p Len %i Buf3 %p Len %i\n",buffer1,buffer1_size,buffer2,buffer2_size,buffer3,buffer3_size);
  spi_dev = new Adafruit_SPIDevice(CS,
                                   4000000,               // frequency
                                   SPI_BITORDER_MSBFIRST, // bit order
                                   SPI_MODE0,             // data mode
                                   spi);
}

/**************************************************************************/
/*!
    @brief default destructor
*/
/**************************************************************************/
Adafruit_EPD::~Adafruit_EPD() {
  assert(0); //Don't uninitialize
}


/**************************************************************************/
/*!
    @brief reset Perform a hardware reset
*/
/**************************************************************************/
void Adafruit_EPD::hardwareReset(void) {
  if (_reset_pin >= 0) {
    // Setup reset pin direction
    pinMode(_reset_pin, OUTPUT);
    // VDD (3.3V) goes high at start, lets just chill for a ms
    digitalWrite(_reset_pin, HIGH);
    delay(10);
    // bring reset low
    digitalWrite(_reset_pin, LOW);
    // wait 10ms
    delay(10);
    // bring out of reset
    digitalWrite(_reset_pin, HIGH);
    delay(10);
  }
}


void Adafruit_EPD::writeTripleRAMFramebufferToEPD(uint8_t *framebuffer1,
                                            uint32_t framebuffer1_size,
											uint8_t *framebuffer2,
                                            uint32_t framebuffer2_size,
											uint8_t *framebuffer3,
                                            uint32_t framebuffer3_size,
                                            uint8_t EPDlocation,
                                            bool invertdata) {
  writeRAMCommand(EPDlocation);
  dcHigh();

  for (uint32_t i = 0; i < framebuffer1_size; i++) {
    uint8_t d = framebuffer1[i];
    if (invertdata)
      d = ~d;
    SPItransfer(d);
  }
  for (uint32_t i = 0; i < framebuffer2_size; i++) {
    uint8_t d = framebuffer2[i];
    if (invertdata)
      d = ~d;
    SPItransfer(d);
  }
  for (uint32_t i = 0; i < framebuffer3_size; i++) {
    uint8_t d = framebuffer3[i];
    if (invertdata)
      d = ~d;
    SPItransfer(d);
  }
  csHigh();
  return;
}


/**************************************************************************/
/*!
 */
/**************************************************************************/
void Adafruit_EPD::EPD_commandList(const uint8_t *init_code) {
  uint8_t buf[64];

  while (init_code[0] != 0xFE) {
    uint8_t cmd = init_code[0];
    init_code++;
    uint8_t num_args = init_code[0];
    init_code++;
    if (cmd == 0xFF) {
      busy_wait();
      delay(num_args);
      continue;
    }
    if (num_args > sizeof(buf)) {
      Serial.println("ERROR - buf not large enough!");
      while (1)
        delay(10);
    }

    for (int i = 0; i < num_args; i++) {
      buf[i] = init_code[0];
      init_code++;
    }
    EPD_command(cmd, buf, num_args);
  }
}

/**************************************************************************/
/*!
    @brief send an EPD command followed by data
    @param c the command to send
    @param buf the buffer of data to send
    @param len the length of the data buffer
*/
/**************************************************************************/
void Adafruit_EPD::EPD_command(uint8_t c, const uint8_t *buf, uint16_t len) {
  EPD_command(c, false);
  EPD_data(buf, len);
}

/**************************************************************************/
/*!
    @brief send an EPD command with no data
    @param c the command to send
    @param end if true the cs pin will be pulled high following the transaction.
   If false the cs pin will remain low.
    @returns the data byte read over the SPI bus
*/
/**************************************************************************/
uint8_t Adafruit_EPD::EPD_command(uint8_t c, bool end) {
  // SPI
  csHigh();
  dcLow();
  csLow();

  uint8_t data = SPItransfer(c);

  if (end) {
    csHigh();
  }

  return data;
}

/**************************************************************************/
/*!
    @brief send data to the display
    @param buf the data buffer to send
    @param len the length of the data buffer
*/
/**************************************************************************/
void Adafruit_EPD::EPD_data(const uint8_t *buf, uint16_t len) {
  // SPI
  dcHigh();

  for (uint16_t i = 0; i < len; i++) {
    SPItransfer(buf[i]);
  }
  csHigh();
}

/**************************************************************************/
/*!
    @brief send data to the display
    @param data the data byte to send
*/
/**************************************************************************/
void Adafruit_EPD::EPD_data(uint8_t data) {
  // SPI
  csHigh();
  dcHigh();
  csLow();

  SPItransfer(data);

  csHigh();
}

/**************************************************************************/
/*!
    @brief transfer a single byte over SPI.
    @param d the data to send
    @returns the data byte read
*/
/**************************************************************************/
uint8_t Adafruit_EPD::SPItransfer(uint8_t d) {
  if (singleByteTxns) {
    uint8_t b;
    csLow();
    b = spi_dev->transfer(d);
    csHigh();
    return b;
  } else {
    return spi_dev->transfer(d);
  }
}

/**************************************************************************/
/*!
    @brief set chip select pin high
*/
/**************************************************************************/
void Adafruit_EPD::csHigh() {
  digitalWrite(_cs_pin, HIGH);

  if (_isInTransaction) {
    spi_dev->endTransaction();
    _isInTransaction = false;
  }
}

/**************************************************************************/
/*!
    @brief set chip select pin low
*/
/**************************************************************************/
void Adafruit_EPD::csLow() {

  if (!_isInTransaction) {
    spi_dev->beginTransaction();
    _isInTransaction = true;
  }

  digitalWrite(_cs_pin, LOW);
}

/**************************************************************************/
/*!
    @brief set data/command pin high
*/
/**************************************************************************/
void Adafruit_EPD::dcHigh() {
  digitalWrite(_dc_pin, HIGH);
}

/**************************************************************************/
/*!
    @brief set data/command pin low
*/
/**************************************************************************/
void Adafruit_EPD::dcLow() {
  digitalWrite(_dc_pin, LOW);
}

void Adafruit_EPD::clearBuffer() {
  memset(buffer1, 0x11, buffer1_size);
	memset(buffer2, 0x11, buffer2_size);
	memset(buffer3, 0x11, buffer3_size);
}


/**************************************************************************/
/*!
    @brief clear the display twice to remove any spooky ghost images
*/
/**************************************************************************/
void Adafruit_EPD::clearDisplay() {
  clearBuffer();
  display();
}

/**************************************************************************/
/*!
    @brief draw a single pixel on the screen
        @param x the x axis position
        @param y the y axis position
        @param color the color of the pixel
*/
/**************************************************************************/
void Adafruit_EPD::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
    return;
  
  uint8_t *pBuf = 0;
  uint32_t addr = ((uint32_t)x + (uint32_t)y * width) / 2;
  if (addr < buffer1_size)
  {
	  pBuf = buffer1;
  }
  else if(addr < buffer1_size + buffer2_size)
  {
	  addr -= buffer1_size;
	  pBuf = buffer2;
  }
  else
  {
	  addr -= buffer1_size;
	  addr -= buffer2_size;
	  pBuf = buffer3;
  }

  pBuf = pBuf + addr;
  bool lower_nibble = x % 2;
  if (lower_nibble) {
    *pBuf &= 0xF0; // save higher nib
    *pBuf |= (color & 0xF);
  } else {
    *pBuf &= 0x0F; // save lower nib
    *pBuf |= (color & 0xF) << 4;
  }
}

uint8_t Adafruit_EPD::getPixel(int16_t x, int16_t y)
{
  if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
    return 0;
  
  uint8_t *pBuf = 0;
  uint32_t addr = ((uint32_t)x + (uint32_t)y * width) / 2;
  if (addr < buffer1_size)
  {
	  pBuf = buffer1;
  }
  else if(addr < buffer1_size + buffer2_size)
  {
	  addr -= buffer1_size;
	  pBuf = buffer2;
  }
  else
  {
	  addr -= buffer1_size;
	  addr -= buffer2_size;
	  pBuf = buffer3;
  }

  pBuf = pBuf + addr;
  bool lower_nibble = x % 2;
  if (lower_nibble) {
    return *pBuf & 0x0F;
  } else {
    return *pBuf >> 4;
  }
}

/**************************************************************************/
/*!
    @brief wait for busy signal to end
*/
/**************************************************************************/
void Adafruit_EPD::busy_wait(void) {
  if (_busy_pin >= 0) {
    while (!digitalRead(_busy_pin)) { // wait for busy high
      delay(10);
    }
  } else {
    delay(BUSY_WAIT);
  }
}

/**************************************************************************/
/*!
    @brief begin communication with and set up the display.
    @param reset if true the reset pin will be toggled.
*/
/**************************************************************************/
void Adafruit_EPD::begin(bool reset) {
  pinMode(_dc_pin, OUTPUT);
  pinMode(_cs_pin, OUTPUT);

  csHigh();

  if (!spi_dev->begin()) {
    return;
  }

  if (reset) {
    hardwareReset();
  }

  if (_busy_pin >= 0) {
    pinMode(_busy_pin, INPUT);
  }
  delay(100);
}

/**************************************************************************/
/*!
    @brief Transfer the data stored in the buffer(s) to the display
*/
/**************************************************************************/
void Adafruit_EPD::display(bool sleep) {
  powerUp();
  writeTripleRAMFramebufferToEPD(buffer1, buffer1_size, buffer2, buffer2_size, buffer3, buffer3_size, 0);
  update();
  if (sleep) {
    powerDown();
  }
}

/**************************************************************************/
/*!
    @brief signal the display to update
*/
/**************************************************************************/
void Adafruit_EPD::update(void) {
  EPD_command(ACEP_POWER_ON);
  busy_wait();
  EPD_command(ACEP_DISPLAY_REFRESH);
  busy_wait();
  EPD_command(ACEP_POWER_OFF);
  if (_busy_pin >= 0) {
    while (digitalRead(_busy_pin)) { // wait for busy LOW
      delay(10);
    }
  } else {
    delay(BUSY_WAIT);
  }
}

/**************************************************************************/
/*!
    @brief start up the display
*/
/**************************************************************************/
void Adafruit_EPD::powerUp() {
  uint8_t buf[5];

  hardwareReset();
  delay(200);
  busy_wait();
  const uint8_t *init_code = acep_default_init_code;

  if (_epd_init_code != NULL) {
    init_code = _epd_init_code;
  }
  EPD_commandList(init_code);

  // set resolution
  buf[0] = 0x02;
  buf[1] = 0x58;
  buf[2] = 0x01;
  buf[3] = 0xC0;
  EPD_command(ACEP_RESOLUTION, buf, 4);

  delay(100);
}

/**************************************************************************/
/*!
    @brief wind down the display
*/
/**************************************************************************/

void Adafruit_EPD::powerDown(void) {
  uint8_t buf[1];

  delay(1000);

  // deep sleep
  buf[0] = 0xA5;
  EPD_command(ACEP_DEEP_SLEEP, buf, 1);

  delay(100);
}

/**************************************************************************/
/*!
    @brief Send the specific command to start writing to EPD display RAM
    @param index The index for which buffer to write (0 or 1 or tri-color
   displays) Ignored for monochrome displays.
    @returns The byte that is read from SPI at the same time as sending the
   command
*/
/**************************************************************************/
uint8_t Adafruit_EPD::writeRAMCommand(uint8_t index) {
  (void)index;
  return EPD_command(ACEP_DTM, false);
}

/**************************************************************************/
/*!
    @brief Some displays require setting the RAM address pointer
    @param x X address counter value
    @param y Y address counter value
*/
/**************************************************************************/
void Adafruit_EPD::setRAMAddress(uint16_t x, uint16_t y) {
  (void)x;
  (void)y;
}