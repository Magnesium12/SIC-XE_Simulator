#include<fstream>
#include<iostream>

using namespace std;

ifstream input;
ofstream output,intermediate,error,symtab,littab;

void open_files()
{
    input.open("input.txt");
    output.open("output.txt");
    intermediate.open("intermediate.txt");
    error.open("error.txt");
    symtab.open("symtab.txt");
    littab.open("littab.txt");
}

void check_files()
{
    if(!input.is_open())
    {
        cout<<"Input file not found"<<endl;
        exit(0);
    }
    if(!output.is_open())
    {
        cout<<"Output file not found"<<endl;
        exit(0);
    }
    if(!intermediate.is_open())
    {
        cout<<"Intermediate file not found"<<endl;
        exit(0);
    }
    if(!error.is_open())
    {
        cout<<"Error file not found"<<endl;
        exit(0);
    }
    if(!symtab.is_open())
    {
        cout<<"Symbol table file not found"<<endl;
        exit(0);
    }
    if(!littab.is_open())
    {
        cout<<"Literal table file not found"<<endl;
        exit(0);
    }
}

void close_files()
{
    input.close();
    output.close();
    intermediate.close();
    error.close();
    symtab.close();
    littab.close();
}