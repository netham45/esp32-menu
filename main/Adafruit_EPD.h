/*!
 * @file Adafruit_EPD.h
 *
 * This is a library for our EPD displays based on EPD drivers.
 * Designed specifically to work with Adafruit EPD displays.
 *
 * These displays use SPI to communicate, 4 or 5 pins are required to
 * interface
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef _ADAFRUIT_EPD_H_
#define _ADAFRUIT_EPD_H_

#define ACEP_PANEL_SETTING 0x00
#define ACEP_POWER_SETTING 0x01
#define ACEP_POWER_OFF 0x02
#define ACEP_POWER_OFF_SEQUENCE 0x03
#define ACEP_POWER_ON 0x04
#define ACEP_BOOSTER_SOFT_START 0x06
#define ACEP_DEEP_SLEEP 0x07
#define ACEP_DTM 0x10
#define ACEP_DISPLAY_REFRESH 0x12
#define ACEP_PLL 0x30
#define ACEP_TSE 0x40
#define ACEP_CDI 0x50
#define ACEP_TCON 0x60
#define ACEP_RESOLUTION 0x61
#define ACEP_PWS 0xE3

#define ACEP_COLOR_BLACK 0x0  /// 000
#define ACEP_COLOR_WHITE 0x1  ///	001
#define ACEP_COLOR_GREEN 0x2  ///	010
#define ACEP_COLOR_BLUE 0x3   ///	011
#define ACEP_COLOR_RED 0x4    ///	100
#define ACEP_COLOR_YELLOW 0x5 ///	101
#define ACEP_COLOR_ORANGE 0x6 ///	110

//#define EPD_DEBUG

#define RAMBUFSIZE 64 ///< size of the ram buffer

#include <Adafruit_SPIDevice.h>

/**************************************************************************/
/*!
    @brief available EPD colors
*/
/**************************************************************************/
enum
{
    EPD_WHITE, ///< white color
    EPD_BLACK, ///< black color
    EPD_RED,   ///< red color
    EPD_GRAY,  ///< gray color ('red' on grayscale)
    EPD_DARK,  ///< darker color
    EPD_LIGHT, ///< lighter color
    EPD_NUM_COLORS
};

#define EPD_swap(a, b) \
    {                  \
        int16_t t = a; \
        a = b;         \
        b = t;         \
    } ///< simple swap function

/**************************************************************************/
/*!
    @brief  Class for interfacing with Adafruit EPD display breakouts.
*/
/**************************************************************************/
class Adafruit_EPD
{
public:
    Adafruit_EPD(int width, int height, int16_t DC, int16_t RST, int16_t CS,
                 int16_t BUSY = -1, SPIClass *spi = &SPI);
    ~Adafruit_EPD();

    void begin(bool reset = true);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void clearBuffer();
    void clearDisplay();
    void display(bool sleep = false);

protected:
    void writeTripleRAMFramebufferToEPD(uint8_t *buffer1, uint32_t buffer1_size,
                                        uint8_t *buffer2, uint32_t buffer2_size,
                                        uint8_t *buffer3, uint32_t buffer3_size,
                                        uint8_t EPDlocation, bool invertdata = false);

    /**************************************************************************/
    /*!
      @brief Send the specific command to start writing to EPD display RAM
      @param index The index for which buffer to write (0 or 1 or tri-color
      displays) Ignored for monochrome displays.
      @returns The byte that is read from SPI at the same time as sending the
      command
    */
    /**************************************************************************/
    uint8_t writeRAMCommand(uint8_t index);

    /**************************************************************************/
    /*!
      @brief Some displays require setting the RAM address pointer
      @param x X address counter value
      @param y Y address counter value
    */
    /**************************************************************************/
    void setRAMAddress(uint16_t x, uint16_t y);

    void busy_wait(void);

    /**************************************************************************/
    /*!
      @brief start up the display
    */
    /**************************************************************************/
    void powerUp();

    /**************************************************************************/
    /*!
      @brief signal the display to update
    */
    /**************************************************************************/
    void update(void);

    /**************************************************************************/
    /*!
      @brief wind down the display
    */
    /**************************************************************************/
    void powerDown(void);
    void hardwareReset(void);

    int16_t _dc_pin,                    ///< data/command pin
        _reset_pin,                     ///< reset pin
        _cs_pin,                        ///< chip select pin
        _busy_pin;                      ///< busy pin
    Adafruit_SPIDevice *spi_dev = NULL; ///< SPI object
    static bool _isInTransaction;       ///< true if SPI bus is in trasnfer state
    bool singleByteTxns;                ///< if true CS will go high after every data byte
                                        ///< transferred

    const uint8_t *_epd_init_code = NULL;
    const uint8_t *_epd_lut_code = NULL;

    uint16_t default_refresh_delay = 15000;

    uint32_t buffer1_size; ///< size of the primary buffer
    uint32_t buffer2_size; ///< size of the secondary buffer
    uint32_t buffer3_size;
    uint8_t *buffer1;
    uint8_t *buffer2;
    uint8_t *buffer3;
    uint32_t width;
    uint32_t height;

    void EPD_commandList(const uint8_t *init_code);
    void EPD_command(uint8_t c, const uint8_t *buf, uint16_t len);
    uint8_t EPD_command(uint8_t c, bool end = true);
    void EPD_data(const uint8_t *buf, uint16_t len);
    void EPD_data(uint8_t data);

    uint8_t SPItransfer(uint8_t c);

    void csLow();
    void csHigh();
    void dcHigh();
    void dcLow();
};

#endif /* _ADAFRUIT_EPD_H_ */
