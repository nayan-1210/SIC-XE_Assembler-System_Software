I have implemented 2 pass SIC/XE Assembler supporting Program Block in C++ language. 
This Assembler converts assembly code written in SIC/XE into object program. 

Input File (Pass 1) 
1. input.txt: Contains the assembly code in SIC/XE language.

Output File (Pass 1) 
1. intermediate.txt: Contains useful information about instruction, its corresponding 
program block. 
2. symtab.txt: Symbol Table - Contains mapping of labels, literals and their location 
counter, program block, and its relative/absolute status. 
3. error_pass1.txt: Contains errors and location of errors that are found while 
performing pass1.

Input File (Pass 2) 
1. intermediate.txt: Generated from pass 1.
2. symtab.txt: Generated from pass 1.

Output File (Pass 2) 
1. listingFile.txt: Contains instruction, its line number, memory address after the 
assembly, program block, and assembled object code of every instruction. It also 
contains comments. 
2. objectFile.txt: Contains output generated by the assembler after successfully 
assembling the source code. It also contains the machine code instructions and data, 
organized in a format that can be directly loaded and executed by the target machine 
(in this case, a SIC/XE computer).
It is organized in the following manner. 
a) Header Record: Contains information about name of program, starting address, 
and program length. 
b) Text Record: Contains actual object code generated by the actual program. It also 
contains the starting address of the current text record and its length. 
c) Modification Record: Contains information about the instructions that require 
modifications at specific locations. 
d) End Record: Contains information about the address of the first executable 
instruction. 
3. error_pass2.txt: Contains errors and location of errors that are found while 
performing pass 2.
 
CPP code files 
1. main.cpp: It calls two functions pass1() and pass2() present in pass1.cpp and 
pass2.cpp respectively. 
2. pass1.cpp: Contains the necessary code for performing pass 1 of the assembly. 
3. pass2.cpp: Contains the necessary code for performing pass 2 of the assembly. 
4. pass1.hpp: Contains the shared variable (program length) between both pass1.cpp and 
pass2.cpp. 
5. optab.cpp: Contains the opcode mapping and register mapping.
   
Supporting Features 
1. Literals 
2. Symbol Defining Statements 
3. Expressions 
4. Program Block

Input File (input.txt)

COPY START 0 
FIRST STL RETADR 
CLOOP JSUB RDREC 
 LDA LENGTH 
 COMP #0 
 JEQ ENDFIL 
 JSUB WRREC 
 J CLOOP 
ENDFIL LDA =C'EOF' 
 STA BUFFER 
 LDA #3 
 STA LENGTH 
 JSUB WRREC 
 J @RETADR 
 USE CDATA 
RETADR RESW 1 
LENGTH RESW 1 
 USE CBLKS 
BUFFER RESB 4096 
BUFFEND EQU * 
MAXLEN EQU BUFFEND-BUFFER 
. 
. READ RECORD FROM BUFFER 
. 
 USE 
RDREC CLEAR X 
 CLEAR A 
 CLEAR S 
 +LDT #MAXLEN 
RLOOP TD INPUT 
 JEQ RLOOP 
 RD INPUT 
 COMPR A,S 
 JEQ EXIT 
 STCH BUFFER,X 
 TIXR T 
 JLT RLOOP 
EXIT STX LENGTH 
 RSUB 
 USE CDATA 
INPUT BYTE X'F1' 
. 
. WRITE RECORD INTO BUFFER 
. 
 USE 
WRREC CLEAR X 
 LDT LENGTH 
WLOOP TD =X'05' 
 JEQ WLOOP 
 LDCH BUFFER,X 
 WD =X'05' 
 TIXR T 
 JLT WLOOP 
 RSUB 
 USE CDATA 
 LTORG 
 END FIRST 

   
