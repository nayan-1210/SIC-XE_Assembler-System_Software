#include <bits/stdc++.h>
#include "optab.cpp"
#include "pass1.hpp"
using namespace std;

vector<string> break_line(string line)
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

map<string, pair<char, pair<int, int>>> symtab;
map<string, pair<string, string>> optab;
set<string> littab;
map<string, pair<int, int>> literal_tab;
map<string, int> prog_blk_num;
map<int, int> prog_blk_ctr;
map<int, int> prog_blk_start_addr;

void insert_symtab(string label, int loc_ctr, int blk)
{
    symtab[label] = {'R', {loc_ctr, blk}};
}

int search_symtab(string label)
{
    if (symtab.find(label) != symtab.end())
        return 1;
    else
        return 0;
}

string search_optab(string opcode)
{
    if (optab.find(opcode) != optab.end())
    {
        return optab[opcode].first;
    }
    else
    {
        return "na";
    }
}
ofstream errorFile("error_pass1.txt");
int loc_ctr;
int evaluateExpression(string expression)
{
    stringstream ss(expression);
    string token;
    vector<string> tokens;

    // Splitting the expression into tokens
    while (getline(ss, token, '+'))
    {
        tokens.push_back(token);
    }

    // Evaluating the expression
    int result = 0;
    for (const auto &token : tokens)
    {
        size_t pos = token.find('-');
        if (pos != string::npos)
        { // If token contains '-'
            string var1 = token.substr(0, pos);
            string var2 = token.substr(pos + 1);
            if(symtab.find(var1)==symtab.end()){
                errorFile<<"Label should be defined before it is used as an operand field at location counter(in decimal): "<<loc_ctr<<endl;
                
            }
            if(symtab.find(var2)==symtab.end()){
                errorFile<<"Label should be defined before it is used as an operand field at location counter(in decimal): "<<loc_ctr<<endl;
                
            }
            int val1 = 0;
            int val2 = symtab.at(var2).second.first; // Absolute value
            if (symtab.at(var1).first == 'R')
            { // Relative term
                val1 = symtab.at(var1).second.first;
            }
            result -= val2; // Subtract the second variable
            result += val1; // Add the first variable (if relative)
        }
        else
        {                                            // If token doesn't contain '-'
            int val = symtab.at(token).second.first; // Absolute value
            if (symtab.at(token).first == 'R')
            { // Relative term
                result += val;
            }
        }
    }

    return result;
}
int endflag=0;

void pass1()
{
    cout << "Inside pass1 function" << endl;

    ifstream inputFile("input.txt");
    ofstream intermediateFile("intermediate.txt"), symtabFile("symtab.txt");
    
    // Check if the input file is opened successfully
    if (!inputFile)
    {
        errorFile << "Error: Unable to open the input file!" << endl;
    }
    // Check if the intermediate file is opened successfully
    if (!intermediateFile)
    {
        errorFile << "Error: Unable to open the intermediate file!" << endl;
    }
    // Check if the error file is opened successfully
    if (!errorFile)
    {
        errorFile << "Error: Unable to open the error file!" << endl;
    }
    // Check if the symtab file is opened successfully
    if (!symtabFile)
    {
        errorFile << "Error: Unable to open the symtab file!" << endl;
    }

    
    int starting_address;
    optab = load_opcode();
    // Read and output the file contents line by line
    string line;
    string blk_name = "default";
    loc_ctr=prog_blk_ctr[1];
    int blocks = 1;
    while (getline(inputFile, line))
    {
        // cout<<line<<endl;
        if (line[0] == '.')
        {
            intermediateFile << "-"
                             << " " << line << endl;
            continue;
        }
        vector<string> parse_line = break_line(line);
        string label = parse_line[0];
        string opcode = parse_line[1];
        string operand;
        if (parse_line.size() == 2 || parse_line[2] == "")
            operand = " ";
        else
            operand = parse_line[2];

        if (opcode == "USE")
        {
            if (operand == " ")
            {
                blk_name = "default";
                loc_ctr = prog_blk_ctr[1];
            }
            else
            {
                if (prog_blk_num.find(operand) != prog_blk_num.end())
                {
                    blk_name = operand;
                    loc_ctr = prog_blk_ctr[prog_blk_num[blk_name]];
                }
                else
                {
                    blocks++;
                    blk_name = operand;
                    prog_blk_num[blk_name] = blocks;
                    prog_blk_ctr[blocks] = 0;
                    loc_ctr = 0;
                }
            }
        }

        if (opcode == "START")
        {
            loc_ctr = stoi(operand);
            starting_address = loc_ctr;
            prog_blk_num["default"] = 1;
            prog_blk_ctr[1] = 0;
            prog_blk_start_addr[1] = 0;
            intermediateFile << prog_blk_num[blk_name] << " " << line << endl;
            continue;
        }

        if (opcode == "END")
        {
            endflag=1;
            intermediateFile << "-"
                             << " " << line << endl;
            for (auto literal : littab)
            {
                // cout<<"in ltorg: "<<literal<<" "<<loc_ctr<<endl;
                string literal_line = "* " + literal + " ";
                intermediateFile << prog_blk_num[blk_name] << " " << literal_line << endl;
                literal_tab[literal] = {loc_ctr, prog_blk_num[blk_name]};
                if (literal[1] == 'C')
                {
                    loc_ctr += (literal.length() - 4);
                }
                else
                {
                    loc_ctr += (literal.length() - 4) / 2;
                }
                prog_blk_ctr[prog_blk_num[blk_name]] = loc_ctr;
            }
            program_length = 0;
            for (auto prog_blks : prog_blk_ctr)
            {
                program_length += (prog_blks.second);
            }
            // program_length=loc_ctr-starting_address;

            // cout<<"Program length: "<<program_length<<endl;
            break;
        }

        if (operand[0] == '=')
        {
            littab.insert(operand);

            // intermediateFile<<line<<endl;
        }
        if (opcode == "LTORG")
        {
            intermediateFile << "-"
                             << " " << line << endl;
            for (auto literal : littab)
            {
                // cout<<"in ltorg: "<<literal<<" "<<loc_ctr<<endl;
                string literal_line = "* " + literal + " ";
                intermediateFile << prog_blk_num[blk_name] << " " << literal_line << endl;
                literal_tab[literal] = {loc_ctr, prog_blk_num[blk_name]};
                if (literal[1] == 'C')
                {
                    loc_ctr += (literal.length() - 4);
                }
                else
                {
                    loc_ctr += (literal.length() - 4) / 2;
                }
            }
            prog_blk_ctr[prog_blk_num[blk_name]] = loc_ctr;
            littab.clear();

            continue;
        }
        if (opcode == "EQU")
        {
            if (operand == "*")
            {
                symtab[label] = {'R', {loc_ctr, prog_blk_num[blk_name]}};
                intermediateFile << prog_blk_num[blk_name] << " " << line << endl;
            }
            else
            {
                symtab[label] = {'A', {evaluateExpression(operand), -1}};
                intermediateFile << "-"
                                 << " " << line << endl;
            }

            continue;
        }

        if (label != " ")
        {
            int search = search_symtab(label);
            if (search == 0)
                insert_symtab(label, loc_ctr, prog_blk_num[blk_name]);
            else
            {

                string error_line = "Error: Duplicate Symbol at location counter(in decimal): " + to_string(loc_ctr);
                errorFile << error_line << endl;
                // cout<<"Error: Duplicate Symbol"<<endl;
            }
        }

        // for(auto i:optab){
        //     cout<<i.first<<" "<<i.second.first<<" "<<i.second.second<<endl;
        // }
        string search;
        if (opcode[0] == '+')
        {
            search = search_optab(opcode.substr(1, opcode.length() - 1));
        }
        else
            search = search_optab(opcode);
        if (search != "na")
        {
            if (search == "1")
                loc_ctr += 1;
            else if (search == "2")
                loc_ctr += 2;
            else
            {
                if (opcode[0] == '+')
                    loc_ctr += 4;
                else
                    loc_ctr += 3;
            }
        }
        else if (opcode == "WORD")
        {
            loc_ctr += 3;
        }
        else if (opcode == "RESW")
        {
            loc_ctr += (3 * stoi(operand));
        }
        else if (opcode == "RESB")
        {
            loc_ctr += (stoi(operand));
        }
        else if (opcode == "BYTE")
        {
            if (operand[0] == 'C')
            {
                if (operand[1] != '\'' || operand[operand.length() - 1] != '\'')
                {
                    string error_line = "Error: Invalid operand value for BYTE type instructions at location counter: " + to_string(loc_ctr);
                    errorFile << error_line << endl;
                    // cout<<"Error: Invalid operand value for BYTE type instructions"<<endl;
                }
                else
                    loc_ctr += (operand.length() - 3);
            }
            else if (operand[0] == 'X')
            {
                if (operand[1] != '\'' || operand[operand.length() - 1] != '\'' || (operand.length() - 3) % 2 != 0)
                {
                    string error_line = "Error: Invalid operand value for BYTE type instructions at location counter: " + to_string(loc_ctr);
                    errorFile << error_line << endl;
                    // cout<<"Error: Invalid operand value for BYTE type instructions"<<endl;
                }
                else
                    loc_ctr += ((operand.length() - 3) / 2);
            }
            else
            {
                string error_line = "Error: Invalid format for BYTE type instructions at location counter: " + to_string(loc_ctr);
                errorFile << error_line << endl;
                // cout<<"Error: Invalid format for BYTE type instructions"<<endl;
            }
        }
        else if (opcode == "BASE")
        {
            intermediateFile << prog_blk_num[blk_name] << " " << line << endl;
            continue;
        }
        else if (opcode != "EQU" && opcode != "LTORG" && opcode != "USE")
        {
            string error_line = "Error: Invalid opcode at location counter: " + to_string(loc_ctr);
            errorFile << error_line << endl;
            // cout<<"Error: Invalid opcode"<<endl;
        }

        intermediateFile << prog_blk_num[blk_name] << " " << line << endl;
        prog_blk_ctr[prog_blk_num[blk_name]] = loc_ctr;
    }
    int previous = 0;
    for (auto len : prog_blk_ctr)
    {
        // cout<<len.first<<" : "<<len.second<<endl;
        prog_blk_start_addr[len.first] = previous;
        previous += len.second;
    }

    string symtab_line = "SYMTAB";
    symtabFile << symtab_line << endl;
    symtab_line = "---------------------";
    symtabFile << symtab_line << endl;

    for (auto literal : littab)
    {
        // cout<<literal<<endl;
    }
    for (auto literal : literal_tab)
    {
        symtab[literal.first] = {'R', {literal.second.first, literal.second.second}};
        // cout<<literal.first<<" "<<literal.second.first<<" "<<literal.second.second<<endl;
    }
    for (auto i : symtab)
    {
        symtab_line = i.first + " " + i.second.first + " " + to_string(i.second.second.first) + " " + to_string(i.second.second.second);
        symtabFile << symtab_line << endl;
        // cout<<symtab_line<<endl;
        // cout<<i.first<<" "<<i.second<<endl;
    }
    symtab_line = "---------------------";
    symtabFile << symtab_line << endl;
    symtab_line = "Program length: " + to_string(program_length);
    symtabFile << symtab_line << endl;

    // for(auto len:prog_blk_start_addr){
    //     cout<<len.first<<" : "<<len.second<<endl;
    // }
    if(endflag==0){
        errorFile<<"End statement not found"<<endl;
    }
    // Close the file
    inputFile.close();
    intermediateFile.close();
    errorFile.close();
    symtabFile.close();
}