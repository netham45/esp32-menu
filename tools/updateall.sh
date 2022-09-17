#!/bin/bash
rm -r ../main/src/font
rm -r ../main/src/graphics
rm ../main/font.h ../main/parseddata.cpp ../main/graphics.h

cd font
echo Updating font
./genfont.sh
cp -R output/* ../../main/
cd ../graphics
echo Updating graphics
./genheaders.sh
cp -R output/* ../../main/
cd ../yaml
echo Updating yaml
./genyaml.sh
cp -R output/* ../../main/
cd ../..
chown 100000:100000 -R .
