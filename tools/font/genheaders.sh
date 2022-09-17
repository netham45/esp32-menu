#!/bin/bash
rm -rf output

for i in lcase ucase symbols extended_symbols; do
  mkdir -p output/src/font/$i
  cd $i
  for charbmp in *
  do
    charname=$(echo "$charbmp" | sed 's/.bmp//g')
    xxd -i $charbmp >> ../output/src/font/$i/$charname.cpp
  done
  sed -i 's/unsigned char/extern const unsigned char/g' ../output/src/font/$i/*.cpp
  cd ..
done

cd output

for i in src/font/lcase/*.cpp src/font/ucase/*.cpp src/font/symbols/*.cpp; do
  charname=$(basename $i | sed 's/\.cpp//g')
  echo "extern const unsigned char * __${charname}_bmp;"
done > font.h

echo "const unsigned char* charmap[] = {" >> font.h
for i in $(seq 0 255)
do
  hex=$(printf '%x' $i)
  if find -name "$hex.cpp" | grep "." > /dev/null 2>&1
  then
    echo "__${hex}_bmp," >> font.h
  else
    echo "0," >> font.h
  fi
done

echo "};" >> font.h
