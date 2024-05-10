#include<bits/stdc++.h>
// #include "pass1.cpp"
#include "pass2.cpp"
#include "pass1.hpp"
using namespace std;

int main(){
    cout<<"Source code input is taken from input.txt file in the same folder"<<endl<<endl;
    
    cout<<"Performing Pass 1 on the input source code"<<endl<<endl;

    pass1();
    
    cout<<endl<<"Pass1 is performed successfully and intermediate file is generated in the same folder "<<endl<<endl;
    cout<<"Symbol table is also generated and saved successfully as symtab.txt in the same folder"<<endl<<endl;

    cout<<"Performing Pass 2 on the input source code"<<endl<<endl;
    pass2();

    cout<<endl<<"Pass2 is performed successfully and Object file and Listing file is generated in the same folder "<<endl<<endl;
    

}