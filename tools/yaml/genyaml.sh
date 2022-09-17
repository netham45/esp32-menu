#!/bin/bash
rm -rf output
mkdir output
python3 processyaml.py > output/parseddata.cpp
