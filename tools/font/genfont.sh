#!/bin/bash
rm -- */*.bmp
for char in a b c d e f g h i j k l m n o p q r s t u v w x y z;
do
  hex=$(echo -n "$char" | xxd -ps)
  echo text 0,12 \""$char"\" > temp
  convert -colors 2  -size 9x14 xc:white -font "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMoBd.ttf" -pointsize 14 -fill black +antialias +antialias -draw @temp +antialias ppm:- | pnmdepth 4 | ppmtobmp -windows -bpp 4 > lcase/$hex.bmp 2>/dev/null
done

for char in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z;
do
  hex=$(echo -n "$char" | xxd -ps)
  echo text 0,12 \""$char"\" > temp
  convert -colors 2 -size 9x14 xc:white -font "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMoBd.ttf" -pointsize 14 -fill black +antialias -draw @temp +antialias ppm:- | pnmdepth 4 | ppmtobmp -windows -bpp 4 > ucase/$hex.bmp 2>/dev/null
done

for char in 1 2 3 4 5 6 7 8 9 0 '-' '=' '[' ']' ';' "'" '"' '\' '`' '!' '@' '#' '$' '%' '^' '&' '*' '(' ')' '_' '+' '~'
do
  hex=$(echo -n "$char" | xxd -ps)
  echo text 0,12 \""$char"\" > temp
  convert -colors 2 -size 9x14 xc:white -font "/usr/share/fonts/truetype/ttf-bitstream-vera/VeraMoBd.ttf" -pointsize 14 -fill black +antialias -draw @temp +antialias ppm:- | pnmdepth 4 | ppmtobmp -windows -bpp 4 > symbols/$hex.bmp 2>/dev/null
done

for char in 🔉 🔇 🔊 ⏻ ↩️ ⬆ ⬇ ⬅ ➡ 🔃
do
  hex=$(echo -n "$char" | xxd -ps)
  echo text 0,32 \""$char"\" > temp
  convert -colors 2 -size 32x32 xc:white -font "EmojiSymbols-Regular.woff" -pointsize 42 -fill black +antialias -draw @temp +antialias ppm:- | pnmdepth 4 | ppmtobmp -windows -bpp 4 > extended_symbols/$hex.bmp 2>/dev/null
done
./genheaders.sh
