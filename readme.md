# An SOP-based logic synthesis tool for SRAM-IMC

It is a demo C/C++ project for the manuscript "An Area-Efficient In-Memory Implementation Method of Arbitrary Boolean Function based on SRAM Array", whose author is Sunrui Zhang. 
Source codes and a compiled executable file are contained.

## Requirement
This project is compiled and tested in Windows.
The whole project are compiled under MinGW w64 6.0 (gcc/g++ 8.1.0), and the version of CMake is 3.24.2. 

## Users' Guide: 

1. The synthesis program only allows "pure pla" for inputs. "Pure pla" files are typical .pla format files that not contain any configuration information (.i, .o, .p, etc.) and comments(text starts with "#"). There are some examples of pure pla files in ./resource. Besides, the synthesis program only supports processing single-output functions temporarily. So, please ensure that the pla file for input only contains single output to obtain the correct synthesis result. You can also use the following tool chain to convert a multi-output BLIF file to a single-output PLA file:
i> use poext tool from http://www.ecs.umass.edu/ece/labs/vlsicad/bds/poext.tar.gz
ii> use ABC tool to convert BLIF to PLA
iii>(optional) use TTmin from https://ddd.fit.cvut.cz/www/prj/TT-Min/download.html to simplify the Boolean function

2. The pla_convert.py is a Python script for converting the typical .pla files to the "pure pla" files. Run this script to create a pure pla file from your original pla file.

3. Run "LUT_SRAM_synthesis.exe".
i> The program's outputs are as follows:
-0: Exit
-1: Synthesis process
Input "1" to enter the synthesis process. Inputting 0 or other character will exit the program.
ii> Input the file name of a pure pla below this information:
-Enter the function file name (with suffix) to be processed:
The file name needs to contain file path and the suffix. The examples in ./resource are TXT files.
e.g. ./resource/clipf1.txt
Then program will output the results of logic mergence and spatial mergence.
