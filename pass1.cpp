#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>
#include<vector>
#include<math.h>
#include <algorithm>

using namespace std;

void run_pass1(){
    vector<string> lines;
    string line;
    while(getline(input,line)){
        lines.push_back(line);
    }

    int locctr=0;
    vector<instruction> instructions;
    vector<pair<string,info_literal>> literals;

    // handle all instructions
    for(int i=0;i<lines.size();i++){
        vector<string> words = get_words(lines[i]);
        string label = words[0];
        string mnemonic = words[1];
        string operand = words[2];
        instruction inst(label,mnemonic,operand);

        if(csections.empty()){
            csect section(label,locctr);
            csections.push_back(section);
        }
        //set address of current instruction to previous instruction's address (default)
        if(!csections.back().instructions.empty()){
            inst.address = csections.back().instructions.back().address;
        }
        else{
            inst.address = locctr;
        }

        csections.back().instructions.push_back(inst);
        inst = csections.back().instructions.back();

        if(mnemonic=="START"){
            if(is_hex(operand)){
                locctr = str_to_hex(operand);
            }
            else{
                error << "ERROR: Invalid operand for START" << endl;
                exit(0);
            }
            csections.back().instructions.back().address = locctr;
            csections.back().start = locctr;
            continue;
        }
        else if(mnemonic=="END"){
            csections.back().instructions.pop_back();// remove END instruction to first add literals
            while(!literals.empty()){// add literals
                pair<string,info_literal> lit = literals.back();
                literals.pop_back();
                lit.second.address = locctr;
                csections.back().instructions.push_back(instruction("*",lit.first,""));
                csections.back().instructions.back().address = locctr;
                csections.back().instructions.back().length = lit.second.length;
                csections.back().lit_tab[lit.first] = lit.second;
                locctr += lit.second.length;
            }
            csections.back().end=locctr;
            csections.back().instructions.push_back(inst);// add END instruction again
            break;
        }
        else if(mnemonic=="BYTE"){
            csections.back().instructions.back().address = locctr;
            if(operand[0]=='X'){
                locctr += (operand.length()-2)/2;
                csections.back().instructions.back().length = (operand.length()-2)/2;
            }
            else{
                locctr += operand.length()-3;
                csections.back().instructions.back().length = operand.length()-3;
            }
        }
        else if(mnemonic=="WORD"){
            csections.back().instructions.back().address = locctr;
            csections.back().instructions.back().length = 3;
            locctr += 3;
        }
        else if(mnemonic=="RESW"){
            csections.back().instructions.back().address = locctr;
            if(is_number(operand)){
                csections.back().instructions.back().length = 3*str_to_dec(operand);
                locctr += 3*str_to_dec(operand);
            }
            else{
                error<<"Line "<<i+1<<" : Invalid Operand For RESW Statement";
                exit(0);
            }
        }
        else if(mnemonic=="RESB"){
            csections.back().instructions.back().address = locctr;
            if(is_number(operand)){
                csections.back().instructions.back().length = str_to_dec(operand);
                locctr += str_to_dec(operand);
            }
            else{
                error<<"Line "<<i+1<<" : Invalid Operand For RESB Statement";
                exit(0);
            }
        }
        else if(mnemonic=="BASE"){
            csections.back().valid_base=1;
            continue;
        }
        else if(mnemonic=="NOBASE"){
            csections.back().valid_base=0;
            continue;
        }
        else if(mnemonic=="LTORG"){
            while(!literals.empty()){// add literals
                pair<string,info_literal> lit = literals.back();
                literals.pop_back();
                lit.second.address = locctr;
                csections.back().instructions.push_back(instruction("*",lit.first,""));
                csections.back().instructions.back().address = locctr;
                csections.back().instructions.back().length = lit.second.length;
                csections.back().lit_tab[lit.first] = lit.second;
                locctr += lit.second.length;
            }
            continue;
        }
        else if(mnemonic=="EQU"){
            vector<string> params = find_parameters(operand);
            int plus=0,minus=0;// to check if the operand is valid
            int value=0;// value of the operand
            for(int j=0;j<params.size();j++){
                if(params[j][0]=='+'){
                    if(params[j].substr(1,params[j].length()-1)=="*"){
                        plus++;
                        value += locctr;
                    }
                    else if(is_number(params[j].substr(1,params[j].length()-1))){
                        value += str_to_dec(params[j].substr(1,params[j].length()-1));
                    }
                    else if(csections.back().sym_tab.find(params[j].substr(1,params[j].length()-1))==csections.back().sym_tab.end()){
                        error<<"Line "<<i+1<<" : Symbol Not Found : "<<(params[j].substr(1,params[j].length()-1));
                        exit(0);
                    }
                    else{
                        if(!csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].absolute){
                            plus++;
                        }
                        value += csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].address;
                    }
                }
                else if(params[j][0]=='-'){
                    if(params[j].substr(1,params[j].length()-1)=="*"){
                        minus++;
                        value -= locctr;
                    }
                    else if(is_number(params[j].substr(1,params[j].length()-1))){
                            value -= str_to_dec(params[j].substr(1,params[j].length()-1));
                    }
                    else if(csections.back().sym_tab.find(params[j].substr(1,params[j].length()-1))==csections.back().sym_tab.end()){
                        error<<"Line "<<i+1<<" : Symbol Not Found : "<<(params[j].substr(1,params[j].length()-1));
                        exit(0);
                    }
                    else{
                        if(!csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].absolute){
                            minus++;
                        }
                        value -= csections.back().sym_tab[params[j].substr(1,params[j].length()-1)].address;
                    }
                }
            }
            if((plus-minus)!=0 && (plus-minus)!=1){
                error<<"Line "<<i+1<<" : Invalid Expression For EQU Statement";
                exit(0);
            }
            if(label!="" && csections.back().sym_tab.find(label)==csections.back().sym_tab.end()){
                csections.back().sym_tab[label] = info_sym(value);
            }
            else if(label!="" && csections.back().sym_tab.find(label)!=csections.back().sym_tab.end()){
                error<<"Line "<<i+1<<": Duplicate Symbol: "<<label<<endl;
                exit(0);
            }
            if((plus-minus)==1){
                csections.back().sym_tab[label].absolute = false;
            }
            else{
                csections.back().sym_tab[label].absolute = true;
            }
            continue;
        }
        else if(mnemonic=="ORG"){
            if(is_number(operand)){
                locctr = str_to_dec(operand);
            }
            else if(csections.back().sym_tab.find(operand)!=csections.back().sym_tab.end()){
                locctr = csections.back().sym_tab[operand].address;
            }
            else{
                error<<"Line "<<i+1<<" : Invalid Operand For ORG Statement";
                exit(0);
            }
            continue;
        }
        else if(mnemonic=="CSECT"){
            csections.back().instructions.pop_back();// remove the last instruction to add literals
            while(!literals.empty()){// add literals
                pair<string,info_literal> lit = literals.back();
                literals.pop_back();
                lit.second.address = locctr;
                csections.back().instructions.push_back(instruction("*",lit.first,""));
                csections.back().instructions.back().address = locctr;
                csections.back().instructions.back().length = lit.second.length;
                csections.back().lit_tab[lit.first] = lit.second;
                locctr += lit.second.length;
            }
            csections.back().instructions.push_back(inst);// add the last instruction
            csections.back().end=locctr;

            csect section = csect(label,0);
            csections.push_back(section);
            locctr=0;
            continue;
        }
        else if(mnemonic=="EXTDEF"){
            // this will be handled in the second pass
            continue;
        }
        else if(mnemonic=="EXTREF"){
            vector<string> refs = split_comma(operand);
            for(string s:refs){
                csections.back().ref.push_back(s);
            }
            continue;
        }
        else if(label =="."){
            // comment
            csections.back().instructions.pop_back();
            continue;
        }
        else if(mnemonic==""){
            // blank line
            csections.back().instructions.pop_back();
            error<<"Warning: In line "<<i+1<< ": Blank line found. Start a line with '.' for comments or Leave a space if Label is empty.\n";
            continue;
        }
        else if(OPTAB.find(mnemonic)!=OPTAB.end()){
            csections.back().instructions.back().address = locctr;
            csections.back().instructions.back().length = OPTAB[mnemonic].format;
            locctr += OPTAB[mnemonic].format;
        }
        else{
            error<<"Line "<<i+1<<" : Invalid Operation Code: " << mnemonic << "\n";
            exit(0);
        }
        if(label!="" && csections.back().instructions.back().address!=-0x1){//condition for symbol
            cout << "symbol to be added"<<label<<"\n";
            if(csections.back().sym_tab.find(label)==csections.back().sym_tab.end()){// if symbol not found then add it
                csections.back().sym_tab[label] = info_sym(csections.back().instructions.back().address);
            }
            else{
                error<<"Error: In line "<<i+1<<": Duplicate Symbol"<<endl;
                exit(0);
            }
        }
        if(operand[0]=='='){// condition for literal
            if(csections.back().lit_tab.find(operand)==csections.back().lit_tab.end()){// if literal not found then add it
                csections.back().lit_tab[operand] = info_literal();
                if(operand[1]=='X'){
                    csections.back().lit_tab[operand].length = (operand.length()-3)/2;
                    if(is_hex(operand.substr(3,operand.length()-4))){
                        csections.back().lit_tab[operand].value = str_to_hex(operand.substr(3,operand.length()-4));
                    }
                    else{
                        error<<"Error <In line "<<i+1<<">: Invalid Hexadecimal Literal: " << operand << "\n";
                        exit(0);
                    }
                }
                else if(operand[1]=='C'){
                    csections.back().lit_tab[operand].length = operand.length()-4;
                    csections.back().lit_tab[operand].value = str_to_ascii(operand.substr(3,operand.length()-4));
                }
                else{
                    error<<"Error <In line "<<i+1<<">: Invalid Literal: " << operand << "\n";
                    exit(0);
                }
                literals.push_back({operand,csections.back().lit_tab[operand]});
            }
        }
    }

    // print sym_tab in sym_tab.txt
    for(csect c:csections){
        symtab<<"Symbol Table for "<<c.name<<'\n';
        int MAX_SYMBOL_LENGTH=7,MAX_ADDRESS_LENGTH=7;
        for(auto it = c.sym_tab.begin(); it!=c.sym_tab.end(); it++){
            if(it->first.length()>MAX_SYMBOL_LENGTH)
                MAX_SYMBOL_LENGTH = it->first.length();
            if(to_hex(it->second.address,6).length()>MAX_ADDRESS_LENGTH)
                MAX_ADDRESS_LENGTH = to_hex(it->second.address,6).length();
        }
        symtab<<'\t'<<pad("Symbol",MAX_SYMBOL_LENGTH)<<"\t"<<pad("Address",MAX_ADDRESS_LENGTH)<<"\t"<<"Type"<<'\n';
        for(auto it = c.sym_tab.begin(); it!=c.sym_tab.end(); it++){
            symtab<<'\t'<<pad(it->first,MAX_SYMBOL_LENGTH)<<"\t";
            symtab<<pad(to_hex(it->second.address,6),MAX_ADDRESS_LENGTH)<<"\t";
            if(it->second.absolute){
                symtab<<"Absolute"<<"\n";
            }
            else{
                symtab<<"Relative\n";
            }
        }
        symtab<<'\n';
    }
    // print lit_tab in lit_tab.txt
    for(csect c:csections){
        littab<<"Literal Table for "<<c.name<<'\n';
        int MAX_LITERAL_LENGTH=7,MAX_ADDRESS_LENGTH=7,MAX_LENGTH_LENGTH=6,MAX_VALUE_LENGTH=5;
        for(auto it = c.lit_tab.begin(); it!=c.lit_tab.end(); it++){
            MAX_LITERAL_LENGTH = max(MAX_LITERAL_LENGTH,(int)it->first.length());
            MAX_ADDRESS_LENGTH = max(MAX_ADDRESS_LENGTH,(int)to_hex(it->second.address,6).length());
            MAX_LENGTH_LENGTH = max(MAX_LENGTH_LENGTH,(int)to_string(it->second.length).length());
            MAX_VALUE_LENGTH = max(MAX_VALUE_LENGTH,(int)to_hex(it->second.value,6).length());
        }
        littab<<"\t"<<pad("Literal",MAX_LITERAL_LENGTH)<<"\t"<<pad("Address",MAX_ADDRESS_LENGTH)<<"\t"<<pad("Length",MAX_LENGTH_LENGTH)<<"\t"<<pad("Value",MAX_VALUE_LENGTH)<<"\n";
        for(auto it = c.lit_tab.begin(); it!=c.lit_tab.end(); it++){
            littab<<'\t'<<pad(it->first,MAX_LITERAL_LENGTH)<<"\t";
            littab<<pad(to_hex(it->second.address,6),MAX_ADDRESS_LENGTH)<<"\t"<<pad(to_string(it->second.length),MAX_LENGTH_LENGTH)<<"\t";
            littab<<pad(to_string(it->second.value),MAX_VALUE_LENGTH)<<"\n";
        }
        littab<<'\n';
    }

    // create intermediate file
    for(csect c:csections){
        intermediate<<"Control Section "<<c.name<<'\n';
        int MAX_LABEL_LENGTH=5,MAX_MNEMONIC_LENGTH=8,MAX_OPERAND_LENGTH=7;
        for(instruction inst:c.instructions){
            MAX_LABEL_LENGTH = max(MAX_LABEL_LENGTH,(int)inst.label.length());
            MAX_MNEMONIC_LENGTH = max(MAX_MNEMONIC_LENGTH,(int)inst.mnemonic.length());
            MAX_OPERAND_LENGTH = max(MAX_OPERAND_LENGTH,(int)inst.operand.length());
        }
        intermediate<<"\tAddress "<<pad("Label",MAX_LABEL_LENGTH)<<"\t"<<pad("Mnemonic",MAX_MNEMONIC_LENGTH)<<"\t"<<pad("Operand",MAX_OPERAND_LENGTH)<<"\n";
        for(instruction inst:c.instructions){
            intermediate<<'\t'<<to_hex(inst.address,6)<<"\t"<<pad(inst.label,MAX_LABEL_LENGTH)<<"\t"<<pad(inst.mnemonic,MAX_MNEMONIC_LENGTH)<<"\t"<<pad(inst.operand,MAX_OPERAND_LENGTH)<<"\n";
        }
        intermediate<<'\n';
    }
}
