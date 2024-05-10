#include <bits/stdc++.h>
#include "pass1.hpp"
#include "pass1.cpp"
// #include "optab.cpp"
using namespace std;

map<string, pair<char, pair<int, int>>> read_symtab_pass2_file()
{
    map<string, pair<char, pair<int, int>>> symbolTable;
    ifstream symtab_pass2File("symtab.txt");
    // Check if the symtab_pass2 file is opened successfully
    if (!symtab_pass2File)
    {
        cout << "Error: Unable to open the symtab_pass2 file!" << endl;
    }
    string line;
    while (getline(symtab_pass2File, line))
    {
        istringstream iss(line);
        string symbol;
        char rel;
        int value;
        int blk_no;

        // Read symbol and value from the line
        if (!(iss >> symbol >> rel >> value >> blk_no))
        {
            // cout << "Error: Invalid input line: " << line << endl;
            continue;
        }

        // Insert into the map
        symbolTable[symbol] = {rel, {value, blk_no}};
    }
    return symbolTable;
}

vector<string> break_intermediateline(string line)
{
    vector<string> parse_line;
    // cout<<"Line: "<<line<<endl;
    // cout<<"LIne[0]:"<<line[0]<<":"<<endl;
    if (line[0] == ' ')
        parse_line.push_back(" ");

    int i;
    if (line[0] == ' ')
        i = 1;
    else
        i = 0;

    string part = "";
    while (i < line.length())
    {
        if (line[i] == ' ')
        {
            parse_line.push_back(part);
            part = "";
        }
        else
            part += (line[i]);
        i++;
    }
    parse_line.push_back(part);

    return parse_line;
}

string intTo6Hex(int value)
{
    if (value < 0)
    {
        value = (1 << 24) + value;
    }

    std::stringstream stream;
    stream << uppercase << hex << setw(6) << setfill('0') << value;
    return stream.str();
}
string intToHex(int value, int digits)
{
    if (value < 0)
    {
        value = (1 << 4 * digits) + value;
    }

    std::stringstream stream;
    stream << uppercase << hex << setw(digits) << setfill('0') << value;
    return stream.str();
}

string pad_string(string name)
{
    int left = 6 - name.length();
    while (left > 0)
    {
        name += ' ';
        left--;
    }
    return name;
}

string begin_text_record(int loc_ctr)
{
    string text_record = "T^";
    text_record += (intTo6Hex(loc_ctr));
    text_record += "^  ^";
    return text_record;
}

map<string, pair<string, string>> optab_pass2;
map<string, pair<char, pair<int, int>>> symtab_pass2;
map<char, char> reg_number;
vector<int> mod_record;
map<int, int> prog_blk_ctr_pass2;
map<string, int> prog_blk_num_pass2;

string search_optab_pass2(string opcode)
{
    if (optab_pass2.find(opcode) != optab_pass2.end())
    {
        return optab_pass2[opcode].first;
    }
    else
    {
        
        return "na";
    }
}

string binary_to_hex(string binary_str)
{
    int decimal_value = stoi(binary_str, nullptr, 2);

    stringstream ss;
    ss << uppercase << hex << decimal_value;

    return ss.str();
}

int hexStringToInt(string hex_str)
{
    return stoi(hex_str, nullptr, 16);
}

int search_symtab_pass2(string label)
{
    if (symtab_pass2.find(label) != symtab_pass2.end())
        return 1;
    else
        return 0;
}

string asciiToHex(string input)
{
    stringstream output;
    for (char c : input)
    {
        output << uppercase << hex << setw(2) << setfill('0') << (int)c;
    }
    return output.str();
}
string obj_code = "";
int search_symbol;
string list_line(40, ' ');
string list_line_copy(40, ' ');

void pass2()
{
    cout << "Inside pass2 function" << endl;

    ifstream intermediateFile("intermediate.txt");
    ofstream listingFile("listingFile.txt"), objectFile("objectFile.txt"), errorFile("error_pass2.txt");

    // Check if the intermediate file is opened successfully
    if (!intermediateFile)
    {
        errorFile << "Error: Unable to open the intermediate file!" << endl;
    }
    // Check if the listing file is opened successfully
    if (!listingFile)
    {
        errorFile << "Error: Unable to open the listing file!" << endl;
    }
    // Check if the object file is opened successfully
    if (!objectFile)
    {
        errorFile << "Error: Unable to open the object file!" << endl;
    }
    // Check if the error file is opened successfully
    if (!errorFile)
    {
        errorFile << "Error: Unable to open the error file!" << endl;
    }

    optab_pass2 = load_opcode();
    reg_number = load_register();
    symtab_pass2 = read_symtab_pass2_file();

    vector<string> objectProgram;

    int loc_ctr_pass2 = 0;
    int starting_address;
    string line;
    int left = 60;
    string text_record = begin_text_record(loc_ctr_pass2);
    prog_blk_ctr_pass2[1] = 0;
    loc_ctr_pass2 = prog_blk_ctr_pass2[1];
    int pc = 0;
    int num_blocks = 1;
    string block_name = "default";
    int base;
    int base_blk;
    int prev_loc_ctr = 0;

    while (getline(intermediateFile, line))
    {
        // cout<<line<<endl;
        // cout<<"blockname: "<<block_name<<endl;
        list_line = list_line_copy;
        if (line[2] == '.')
        {

            list_line.replace(7, line.length() - 2, line.substr(2));
            listingFile << list_line << endl;
            continue;
        }

        vector<string> parse_line = break_intermediateline(line);
        int prog_block;
        if (parse_line[0] != "-")
            prog_block = stoi(parse_line[0]);

        string label = parse_line[1];
        string opcode = parse_line[2];
        string operand;
        // cout<<parse_line.size()<<endl;
        // for(auto i:parse_line) cout<<i<<"1";
        // cout<<endl;
        // cout<<parse_line[2]<<"1"<<endl;
        if (parse_line[3] == "" || parse_line.size() == 3)
            operand = " ";
        else
            operand = parse_line[3];
        // cout<<"opcode: "<<opcode<<endl;

        if (opcode == "USE")
        {

            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);
            if (operand == " ")
                block_name = "default";
            else
                block_name = operand;
            // cout<<block_name<<endl;
            // cout<<prog_blk_num_pass2[block_name]<<endl;
            // cout<<prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]]<<endl;
            // cout<<prog_blk_start_addr[prog_blk_num_pass2[block_name]]<<endl;

            left = 60;
            // cout<<"-------------------"<<operand<<"1"<<endl;
            if (operand == " ")
            {
                // cout<<"inside"<<endl;
                block_name = "default";
                loc_ctr_pass2 = prog_blk_ctr_pass2[1];
                text_record = begin_text_record(prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]] + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
            }
            else
            {
                // block_name=operand;
                // loc_ctr=prog_blk_ctr[prog_blk_num[block_name]];
                if (prog_blk_num_pass2.find(operand) != prog_blk_num_pass2.end())
                {
                    block_name = operand;
                    loc_ctr_pass2 = prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]];
                    text_record = begin_text_record(prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]] + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
                }
                else
                {
                    num_blocks++;
                    block_name = operand;
                    prog_blk_num_pass2[block_name] = num_blocks;
                    // cout<<"prog_blk_num_pass2[block_name]= "<<prog_blk_num_pass2[block_name]<<endl;
                    prog_blk_ctr_pass2[num_blocks] = 0;
                    loc_ctr_pass2 = 0;
                    text_record = begin_text_record(prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]] + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
                }
            }
        }

        if (opcode == "BASE")
        {
            base = symtab_pass2[operand].second.first;
            base_blk = symtab_pass2[operand].second.second;
            list_line.replace(0, 4, intToHex(loc_ctr_pass2, 4));
            list_line.replace(5, 1, to_string(prog_blk_num[block_name]));
            list_line.replace(7, 6, list_line_copy.substr(6));
            list_line.replace(14, opcode.length(), opcode);
            list_line.replace(21, operand.length(), operand);
            // list_line.replace(32,obj_code.length(),obj_code);

            listingFile << list_line << endl;
            // listingFile<<line<<endl;
            continue;
        }
        if (opcode == "RESW" || opcode == "RESB" || opcode == "LTORG" || opcode == "EQU" || opcode == "USE")
        {
            if (opcode == "RESW")
            {
                prev_loc_ctr = loc_ctr_pass2;
                pc = loc_ctr_pass2 + (3 * stoi(operand));

                list_line.replace(0, 4, intToHex(loc_ctr_pass2, 4));
                list_line.replace(5, 1, to_string(prog_blk_num[block_name]));
                list_line.replace(7, label.length(), label);
                list_line.replace(14, opcode.length(), opcode);
                list_line.replace(21, operand.length(), operand);
                // list_line.replace(32,obj_code.length(),obj_code);
                listingFile << list_line << endl;
                loc_ctr_pass2 += (3 * stoi(operand));
                prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]] = loc_ctr_pass2;
            }
            else if (opcode == "RESB")
            {
                prev_loc_ctr = loc_ctr_pass2;
                pc = loc_ctr_pass2 + (stoi(operand));

                list_line.replace(0, 4, intToHex(loc_ctr_pass2, 4));
                list_line.replace(5, 1, to_string(prog_blk_num[block_name]));
                list_line.replace(7, label.length(), label);
                list_line.replace(14, opcode.length(), opcode);
                list_line.replace(21, operand.length(), operand);
                // list_line.replace(32,obj_code.length(),obj_code);
                listingFile << list_line << endl;
                loc_ctr_pass2 += (stoi(operand));
                prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]] = loc_ctr_pass2;
            }
            else
            {

                if (opcode != "LTORG")
                {
                    list_line.replace(0, 4, intToHex(loc_ctr_pass2, 4));
                }

                list_line.replace(5, 1, to_string(prog_blk_num[block_name]));
                list_line.replace(7, label.length(), label);
                list_line.replace(14, opcode.length(), opcode);
                list_line.replace(21, operand.length(), operand);
                if (opcode == "EQU")
                {
                    if (symtab_pass2[label].first == 'A')
                        list_line.replace(5, 1, " ");
                }
                // list_line.replace(32,obj_code.length(),obj_code);
                listingFile << list_line << endl;
                prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]] = loc_ctr_pass2;
                continue;
            }

            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);
            text_record = begin_text_record(loc_ctr_pass2 + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
            left = 60;
            // text_record+=(obj_code+"^");
            // left=60-obj_code.length();
            continue;
        }

        // if(line=="CLOOP +JSUB RDREC"){
        //     cout<<"line: ";

        // }
        // cout<<"line: ";
        // cout<<label<<"1"<<opcode<<"1"<<operand<<"1"<<endl;

        if (opcode == "START")
        {
            prog_blk_num_pass2["default"] = 1;
            string header_record = "H^";
            header_record += (pad_string(label) + "^");
            header_record += (intTo6Hex(stoi(operand)) + "^");
            starting_address = stoi(operand);
            loc_ctr_pass2 = stoi(operand);
            header_record += intTo6Hex(program_length);
            objectProgram.push_back(header_record);
            list_line.replace(0, 4, intToHex(loc_ctr_pass2, 4));
            list_line.replace(5, 1, to_string(prog_blk_num[block_name]));
            list_line.replace(7, label.size(), label);
            list_line.replace(14, opcode.length(), opcode);
            list_line.replace(21, operand.length(), operand);
            listingFile << list_line << endl;
            continue;
        }

        if (opcode == "END")
        {
            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);

            list_line.replace(14, opcode.length(), opcode);
            list_line.replace(21, operand.length(), operand);
            listingFile << list_line << endl;

            string end_record = "E^";
            end_record += intTo6Hex((symtab[operand].second.first));
            // cout<<end_record<<endl;
            objectProgram.push_back(end_record);
            break;
        }

        string search;
        if (opcode[0] == '+')
        {
            search = search_optab_pass2(opcode.substr(1, opcode.length() - 1));
        }
        else
            search = search_optab_pass2(opcode);

        // cout<<"search= "<<search<<endl;
        int ni;
        string xbpe = "0000";
        if (search != "na")
        {
            if (search == "1")
                pc = loc_ctr_pass2 + 1;
            else if (search == "2")
                pc = loc_ctr_pass2 + 2;
            else
            {
                if (opcode[0] == '+')
                {
                    // cout<<"inside format 4 "<<endl;
                    pc = loc_ctr_pass2 + 4;
                    xbpe[3] = '1';
                }
                else
                    pc = loc_ctr_pass2 + 3;
            }

            if (search == "1")
            {
                obj_code = optab_pass2[opcode].second;
            }
            else if (search == "2")
            {
                if (operand.length() == 1)
                {
                    string reg_number_str(1, reg_number[operand[0]]);
                    obj_code = optab_pass2[opcode].second + reg_number_str + "0";
                }
                else
                {
                    string reg_number_str1(1, reg_number[operand[0]]);
                    string reg_number_str2(1, reg_number[operand[2]]);
                    obj_code = optab_pass2[opcode].second + reg_number_str1 + reg_number_str2;
                }
            }
            else
            {
                if (opcode[0] == '+')
                {
                    // cout<<"inside format 4 again "<<endl;
                    if (operand != " ")
                    {
                        if (operand.substr(operand.length() - 2, 2) == ",X")
                        {
                            xbpe[0] = '1';
                            operand = operand.substr(0, operand.length() - 2);
                        }

                        if (operand[0] == '@')
                        {
                            ni = 2;
                            search_symbol = search_symtab_pass2(operand.substr(1, operand.length() - 1));
                        }
                        else if (operand[0] == '#')
                        {
                            ni = 1;
                            search_symbol = search_symtab_pass2(operand.substr(1, operand.length() - 1));
                        }
                        else if (operand[0] != '=')
                        {
                            // cout<<"inside ni=3 "<<endl;
                            ni = 3;
                            search_symbol = search_symtab_pass2(operand);
                        }
                        // cout<<"searchsymbol "<<search_symbol<<endl;
                        if (search_symbol == 1)
                        {
                            int symbol_address = symtab_pass2[operand].second.first + prog_blk_start_addr[symtab_pass2[operand].second.second];
                            char symbol_type = symtab_pass2[operand].first;

                            string hex_xbpe = binary_to_hex(xbpe);
                            // cout<<symbol_address<<" "<<hex_xbpe<<endl;
                            // cout<<optab_pass2[opcode.substr(1,opcode.length()-1)].second<<endl;
                            // cout<<intToHex(hexStringToInt(optab_pass2[opcode].second)+ni,2)<<endl;
                            // cout<<intToHex(symbol_address,5)<<endl;
                            obj_code = intToHex(hexStringToInt(optab_pass2[opcode.substr(1, opcode.length() - 1)].second) + ni, 2) + hex_xbpe + intToHex(symbol_address, 5);
                        }
                        else
                        {
                            int difference;
                            if (operand[0] == '@' || operand[0] == '#')
                            {
                                difference = stoi(operand.substr(1, operand.length() - 1));
                            }
                            else
                                difference = stoi(operand);
                            string hex_xbpe = binary_to_hex(xbpe);
                            obj_code = intToHex(hexStringToInt(optab_pass2[opcode.substr(1, opcode.length() - 1)].second) + ni, 2) + hex_xbpe + intToHex(difference, 5);
                        }
                    }
                    else
                    {
                        obj_code = intToHex(hexStringToInt(optab_pass2[opcode.substr(1, opcode.length() - 1)].second) + 3, 2) + "000000";
                        // obj_code=optab_pass2[opcode.substr(1,opcode.length()-1)].second+"000000";
                    }
                }
                else
                {
                    // cout<<"inside format 3 again"<<endl;
                    // cout<<"hello"<<operand<<"hello"<<endl;
                    if (operand != " ")
                    {
                        if (operand.substr(operand.length() - 2, 2) == ",X")
                        {
                            xbpe[0] = '1';
                            operand = operand.substr(0, operand.length() - 2);
                        }

                        if (operand[0] == '@')
                        {
                            ni = 2;
                            search_symbol = search_symtab_pass2(operand.substr(1, operand.length() - 1));
                            operand = operand.substr(1);
                        }
                        else if (operand[0] == '#')
                        {
                            ni = 1;
                            search_symbol = search_symtab_pass2(operand.substr(1, operand.length() - 1));
                            operand = operand.substr(1);
                        }
                        else if (operand[0] != '=')
                        {
                            ni = 3;
                            search_symbol = search_symtab_pass2(operand);
                        }

                        if (search_symbol == 1)
                        {
                            int symbol_address = symtab_pass2[operand].second.first + prog_blk_start_addr[symtab_pass2[operand].second.second];
                            char symbol_type = symtab_pass2[operand].first;
                            // cout<<"sym addr: "<<symbol_address<<endl;
                            pc = loc_ctr_pass2 + 3;
                            // cout<<"pc: "<<pc<<endl;
                            if (symbol_type == 'R')
                            {
                                int difference = symbol_address - (pc + prog_blk_start_addr[prog_blk_num[block_name]]);
                                // cout<<symbol_address<<(pc+prog_blk_start_addr[prog_blk_num[block_name]])<<endl;
                                if (difference >= -2048 && difference < 2047)
                                {
                                    xbpe[2] = '1';
                                }
                                else if(difference<=4095 && difference>=0)
                                {
                                    if(symtab_pass2.find("BASE")==symtab_pass2.end()){
                                        errorFile<<"Error: Assembler directive BASE not found for handling large displacement"<<endl;
                                    }
                                    xbpe[1] = '1';
                                    difference = symbol_address - (base + prog_blk_start_addr[base_blk]);
                                }
                                else{
                                    errorFile<<"Error: For relative addressing using BASE, the displacement is out of bounds"<<endl;
                                }

                                string hex_xbpe = binary_to_hex(xbpe);
                                
                                obj_code = intToHex(hexStringToInt(optab_pass2[opcode].second) + ni, 2) + hex_xbpe + intToHex(difference, 3);
                            }
                            else
                            {
                                string hex_xbpe = binary_to_hex(xbpe);
                                obj_code = intToHex(hexStringToInt(optab_pass2[opcode].second) + ni, 2) + hex_xbpe + intToHex(symbol_address, 3);
                            }
                        }
                        else
                        {
                            int difference;
                            if (operand[0] == '@' || operand[0] == '#')
                            {
                                difference = stoi(operand.substr(1, operand.length() - 1));
                            }
                            else
                                difference = stoi(operand);
                            string hex_xbpe = binary_to_hex(xbpe);
                            obj_code = intToHex(hexStringToInt(optab_pass2[opcode].second) + ni, 2) + hex_xbpe + intToHex(difference, 3);
                        }
                    }
                    else
                    {
                        // cout<<"inside rsub "<<endl;
                        obj_code = intToHex(hexStringToInt(optab_pass2[opcode].second) + 3, 2) + "0000";
                    }
                }
            }
            prev_loc_ctr = loc_ctr_pass2;
            if (search == "1")
                loc_ctr_pass2 += 1;
            else if (search == "2")
                loc_ctr_pass2 += 2;
            else
            {
                if (opcode[0] == '+')
                    loc_ctr_pass2 += 4;
                else
                    loc_ctr_pass2 += 3;
            }
        }
        else if (opcode == "BYTE")
        {
            if (operand[0] == 'C')
            {
                prev_loc_ctr = loc_ctr_pass2;
                pc = loc_ctr_pass2 + (operand.length() - 3);
                obj_code = asciiToHex(operand.substr(2, operand.length() - 3));
                loc_ctr_pass2 += (operand.length() - 3);
            }
            else
            {
                prev_loc_ctr = loc_ctr_pass2;
                pc = loc_ctr_pass2 + (operand.length() - 3) / 2;
                obj_code = operand.substr(2, operand.length() - 3);
                loc_ctr_pass2 += (operand.length() - 3) / 2;
            }
        }
        else if (opcode == "WORD")
        {
            obj_code = intToHex(stoi(operand), 6);
        }
        else if (opcode[0] == '=')
        {
            if (opcode[1] == 'C')
            {
                prev_loc_ctr = loc_ctr_pass2;
                pc = loc_ctr_pass2 + (opcode.length() - 4);
                obj_code = asciiToHex(opcode.substr(3, opcode.length() - 4));
                loc_ctr_pass2 += (opcode.length() - 4);
            }
            else
            {
                prev_loc_ctr = loc_ctr_pass2;
                pc = loc_ctr_pass2 + (opcode.length() - 4) / 2;
                obj_code = opcode.substr(3, opcode.length() - 4);
                loc_ctr_pass2 += (opcode.length() - 4) / 2;
            }
        }
        else{
            errorFile<<"Opcode not found at location counter(in dec): "<<loc_ctr_pass2<<endl;
        }

        list_line.replace(0, 4, intToHex(prev_loc_ctr, 4));
        list_line.replace(5, 1, to_string(prog_blk_num[block_name]));
        list_line.replace(7, label.length(), label);
        list_line.replace(14, parse_line[2].length(), parse_line[2]);
        list_line.replace(21, parse_line[3].length(), parse_line[3]);
        list_line.replace(32, obj_code.length(), obj_code);
        listingFile << list_line << endl;

        if (opcode[0] == '+' && search_symbol == 1)
        {
            // cout<<"modification record"<<endl;
            mod_record.push_back(loc_ctr_pass2 - 4);
        }

        if (obj_code == "")
            continue;
        // cout<<left<<" "<<obj_code<<endl;
        if (obj_code.length() > left)
        {

            int text_rec_length = 60 - left;
            string text_rec_len_str = intToHex(text_rec_length / 2, 2);
            text_record[9] = text_rec_len_str[0];
            text_record[10] = text_rec_len_str[1];
            objectProgram.push_back(text_record);
            if (search == "1")
            {
                text_record = begin_text_record(loc_ctr_pass2 - 1 + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
                // text_record=begin_text_record(loc_ctr_pass2-1);
            }
            else if (search == "2")
            {
                text_record = begin_text_record(loc_ctr_pass2 - 2 + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
                // text_record=begin_text_record(loc_ctr_pass2-2);
            }
            else
            {
                if (opcode[0] == '+')
                {
                    text_record = begin_text_record(loc_ctr_pass2 - 4 + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
                    // text_record=begin_text_record(loc_ctr_pass2-4);
                }
                else
                {
                    text_record = begin_text_record(loc_ctr_pass2 - 3 + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
                    // text_record=begin_text_record(loc_ctr_pass2-3);
                }
            }

            text_record += (obj_code + "^");
            left = 60 - obj_code.length();
        }
        else
        {
            if (left == 60)
                text_record = begin_text_record(prev_loc_ctr + prog_blk_start_addr[prog_blk_num_pass2[block_name]]);
            text_record += (obj_code + "^");
            left -= obj_code.length();
        }

        // cout<<"loc_ctr_pass2: "<<loc_ctr_pass2<<endl;
        // cout<<"block_name: "<<block_name<<endl;
        // cout<<"prog_blk_num_pass2[block_name]: "<<prog_blk_num_pass2[block_name]<<endl;
        prog_blk_ctr_pass2[prog_blk_num_pass2[block_name]] = loc_ctr_pass2;
    }
    for (auto record : objectProgram)
    {
        // objectFile<<record<<endl;
        if (record[0] == 'H' || record[0] == 'E' || record.substr(9, 2) != "00")
            objectFile << record << endl;
    }
}