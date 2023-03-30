# An SOP-based logic synthesis tool for SRAM-IMC

　　It is a demo C/C++ project for the manuscript "An Area-Efficient In-Memory Implementation Method of Arbitrary Boolean Function based on SRAM Array". 

　　File dictionary structure:

　　LUT_SRAM_synthesis
<br>　　├── CMakeLists.txt
<br>　　├── header files //.h files of the program
<br>　　├── LUT_SRAM_synthesis.exe //executable file for Windows
<br>　　├── pla_convert.py //Python script for converting to pure pla
<br>　　├── readme.md
<br>　　├── resource //benchmark examples
<br>　　├── run.sh //shell script for compiling and running the program
<br>　　└── source files //.c and .cpp files of the program
## Requirement
　　gcc/g++ 8.1.0 or higher
<br>　　CMake 3.16.3 or higher
<br>　　Python 3.6.0 or higher
## Users' Guide: 
### Compile and run in Linux
　　```./run.sh```
### Run in Windows
　　Run "LUT_SRAM_synthesis.exe" to use the synthesis tool in Windows.
### Instructions
1. The synthesis program only allows "pure pla" for inputs. "Pure pla" files are typical .pla format files without containing any configuration information (.i, .o, .p, etc.) and comments(text starts with "#"). There are some examples of pure pla files in ./resource. Besides, the synthesis program only supports processing single-output functions temporarily. So, please ensure that the pla file for input only contains single output to obtain the correct synthesis result. You can also use the following tool chain to convert a multi-output BLIF file to a single-output PLA file:
  
　　i> use the poext tool from http://www.ecs.umass.edu/ece/labs/vlsicad/bds/poext.tar.gz
  
　　ii> use the ABC tool to convert BLIF to PLA
  
　　iii>(optional) use TTmin from https://ddd.fit.cvut.cz/www/prj/TT-Min/download.html to simplify the Boolean function

2. The pla_convert.py is a Python script for converting the typical .pla files to the "pure pla" files. Run this script to create a pure pla file from your original pla file.

3. Run the synthesis program.
  
　　i> The program's outputs are as follows:

　　```Choose an item:```
<br>　　```0: Exit```
<br>　　```1: Synthesis process```

　　Input 1 to enter the synthesis process. Input 0 or other character will exit the program.
  
　　ii> Input the file name of a pure pla below this information:

　　```Enter the function file name (with suffix) to be processed:```

　　The file name needs to contain file path and the suffix. The examples in ./resource are TXT files.

　　e.g. ```./resource/clipf1.txt```
  
    iii> Input the output file name below this information:
   
　　```Enter the output file name:```

　　Then program will output the term information, the iteration results of logic mergence and the results of spatial mergence.

#### Contact alwayswry@stu.pku.edu.cn if there is any question.
