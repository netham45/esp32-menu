#!/bin/bash

rm -rf output
mkdir output/src/graphics -p

cd bmps

for charbmp in *.bmp
do
  charname=$(echo "$charbmp" | sed 's/\.bmp//g')
  xxd -i $charbmp >> ../output/src/graphics/$charname.cpp
done


cd ../output/src/graphics

sed -i 's/unsigned char/extern const unsigned char/g' *.cpp

for i in $(ls -v *.cpp | grep -v "graphics.h"); do
  charname=$(echo "$i" | sed 's/\.cpp//g')
  echo "extern const unsigned char * ${charname}_bmp;"
done > ../../graphics.h

cd ..
