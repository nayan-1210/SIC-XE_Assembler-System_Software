#include <bits/stdc++.h>
using namespace std;

map<string, pair<string, string>> load_opcode()
{
    map<string, pair<string, string>> opcode_map;
    opcode_map["ADD"] = {"3/4", "18"};
    opcode_map["ADDF"] = {"3/4", "58"};
    opcode_map["ADDR"] = {"2", "90"};
    opcode_map["AND"] = {"3/4", "40"};
    opcode_map["CLEAR"] = {"2", "B4"};
    opcode_map["COMP"] = {"3/4", "28"};
    opcode_map["COMPF"] = {"3/4", "88"};
    opcode_map["COMPR"] = {"2", "A0"};
    opcode_map["DIV"] = {"3/4", "24"};
    opcode_map["DIVF"] = {"3/4", "64"};
    opcode_map["DIVR"] = {"2", "9C"};
    opcode_map["FIX"] = {"1", "C4"};
    opcode_map["FLOAT"] = {"1", "C0"};
    opcode_map["HIO"] = {"1", "F4"};
    opcode_map["J"] = {"3/4", "3C"};
    opcode_map["JEQ"] = {"3/4", "30"};
    opcode_map["JGT"] = {"3/4", "34"};
    opcode_map["JLT"] = {"3/4", "38"};
    opcode_map["JSUB"] = {"3/4", "48"};
    opcode_map["LDA"] = {"3/4", "00"};
    opcode_map["LDB"] = {"3/4", "68"};
    opcode_map["LDCH"] = {"3/4", "50"};
    opcode_map["LDF"] = {"3/4", "70"};
    opcode_map["LDL"] = {"3/4", "08"};
    opcode_map["LDS"] = {"3/4", "6C"};
    opcode_map["LDT"] = {"3/4", "74"};
    opcode_map["LDX"] = {"3/4", "04"};
    opcode_map["LPS"] = {"3/4", "D0"};
    opcode_map["MUL"] = {"3/4", "20"};
    opcode_map["MULF"] = {"3/4", "60"};
    opcode_map["MULR"] = {"2", "98"};
    opcode_map["NORM"] = {"1", "C8"};
    opcode_map["OR"] = {"3/4", "44"};
    opcode_map["RD"] = {"3/4", "D8"};
    opcode_map["RMO"] = {"2", "AC"};
    opcode_map["RSUB"] = {"3/4", "4C"};
    opcode_map["SHIFTL"] = {"2", "A4"};
    opcode_map["SHIFTR"] = {"2", "A8"};
    opcode_map["SIO"] = {"1", "F0"};
    opcode_map["SSK"] = {"3/4", "EC"};
    opcode_map["STA"] = {"3/4", "0C"};
    opcode_map["STB"] = {"3/4", "78"};
    opcode_map["STCH"] = {"3/4", "54"};
    opcode_map["STF"] = {"3/4", "80"};
    opcode_map["STI"] = {"3/4", "D4"};
    opcode_map["STL"] = {"3/4", "14"};
    opcode_map["STS"] = {"3/4", "7C"};
    opcode_map["STSW"] = {"3/4", "E8"};
    opcode_map["STT"] = {"3/4", "84"};
    opcode_map["STX"] = {"3/4", "10"};
    opcode_map["SUB"] = {"3/4", "1C"};
    opcode_map["SUBF"] = {"3/4", "5C"};
    opcode_map["SUBR"] = {"2", "94"};
    opcode_map["SVC"] = {"2", "B0"};
    opcode_map["TD"] = {"3/4", "E0"};
    opcode_map["TIO"] = {"1", "F8"};
    opcode_map["TIX"] = {"3/4", "2C"};
    opcode_map["TIXR"] = {"2", "B8"};
    opcode_map["WD"] = {"3/4", "DC"};

    return opcode_map;
}


map<char,char> load_register(){
    map<char,char> reg_number;
    reg_number['A']='0';
    reg_number['X']='1';
    reg_number['L']='2';
    reg_number['B']='3';
    reg_number['S']='4';
    reg_number['T']='5';
    reg_number['F']='6';
    return reg_number;
}





