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

void run_pass2(){
    int N = csections.size(); 
    for(int _i=0;_i<N;_i++){
        instruction inst;
        for(int j=0;j<csections[_i].instructions.size();j++){
            inst = csections[_i].instructions[j];

            if(inst.label=="*"){
                if(inst.mnemonic.length()<2){
                    error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid literal: "<<inst.mnemonic<<"\n";
                    exit(0);
                }
                if(inst.mnemonic.substr(0,2)=="=C"){
                    if(inst.mnemonic.length()>34){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Literal too long.\n";
                        exit(0);
                    }
                    inst.obj_code = to_hex(str_to_ascii(inst.mnemonic.substr(3,inst.mnemonic.length()-4)),2*(inst.mnemonic.length()-4));
                }
                else if(inst.mnemonic.substr(0,2)=="=X"){
                    if(inst.mnemonic.length()>19){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Literal too long.\n";
                        exit(0);
                    }
                    inst.obj_code = inst.mnemonic.substr(3,inst.mnemonic.length()-4);
                }
                else{
                    error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid literal: "<<inst.mnemonic<<"\n";
                    exit(0);
                }
            }
            else if(inst.mnemonic=="BYTE" || inst.mnemonic=="WORD"){
                if(inst.operand.substr(0,2)=="C\'"){
                    inst.obj_code = to_hex(str_to_ascii(inst.operand.substr(2,inst.operand.length()-3)),2*(inst.operand.length()-3));
                }
                else if(inst.operand.substr(0,2)=="X\'"){
                    inst.obj_code = inst.operand.substr(2,inst.operand.length()-3);
                }
                else if(is_number(inst.operand)){
                    if(str_to_dec(inst.operand)>pow(2,23)-1 || str_to_dec(inst.operand)<pow(2,23)*(-1)){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Value too large.\n";
                        exit(0);
                    }
                    inst.obj_code = to_hex(str_to_dec(inst.operand),6);
                }
                else if(inst.mnemonic=="WORD"){
                    vector<string> params = find_parameters(inst.operand);
                    if(params.back()=="**"){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid Expression For WORD Statement";
                        exit(0);
                    }
                    int value=0;
                    for(int p=0;p<params.size();p++){
                        if(params[p][0]=='+'){
                            if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))!=csections[_i].ref.end()){
                                csections[_i].mod_record.push_back(info_mod(inst.address,2*inst.length,params[p].substr(1,params[p].length()-1),1));
                            }
                            if(params[p].substr(1,params[j].length()-1)=="*"){
                                value += inst.address+inst.length;
                            }
                            else if(is_number(params[p].substr(1,params[p].length()-1))){
                                value += str_to_dec(params[p].substr(1,params[p].length()-1));
                            }
                            else if(csections[_i].sym_tab.find(params[p].substr(1,params[p].length()-1))!=csections[_i].sym_tab.end()){
                                value += csections[_i].sym_tab[params[p].substr(1,params[p].length()-1)].address;
                            }
                            else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))==csections[_i].ref.end()){
                                error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Symbol Not Found : "<<(params[p].substr(1,params[p].length()-1));
                                exit(0);
                            }
                        }
                        else if(params[p][0]=='-'){
                            if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))!=csections[_i].ref.end()){
                                csections[_i].mod_record.push_back(info_mod(inst.address,2*inst.length,params[p].substr(1,params[p].length()-1),0));
                            }
                            if(params[p].substr(1,params[p].length()-1)=="*"){
                                value -= inst.address+inst.length;
                            }
                            else if(is_number(params[p].substr(1,params[p].length()-1))){
                                    value -= str_to_dec(params[p].substr(1,params[p].length()-1));
                            }
                            else if(csections[_i].sym_tab.find(params[p].substr(1,params[p].length()-1))!=csections[_i].sym_tab.end()){
                                value -= csections[_i].sym_tab[params[p].substr(1,params[p].length()-1)].address;
                            }
                            else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))==csections[_i].ref.end()){
                                error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Symbol Not Found : "<<(params[p].substr(1,params[p].length()-1));
                                exit(0);
                            }
                        }
                    }
                    inst.obj_code = to_hex(value,6);
                }
                else{
                    error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid value for BYTE or WORD\n";
                    exit(0);
                }
            }
            else if(inst.mnemonic=="EXTDEF"){
                vector<string> defs = split_comma(inst.operand);
                for(int i=0;i<defs.size();i++){
                    if(defs[i].length()>6){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : External definition too long.\n";
                        exit(0);
                    }
                    else if(csections[_i].sym_tab.find(defs[i])==csections[_i].sym_tab.end()){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : External definition not found.\n";
                        exit(0);
                    }
                    else{
                        csections[_i].def[defs[i]] = csections[_i].sym_tab[defs[i]].address;
                    }
                }
            }
            else if(OPTAB.find(inst.mnemonic)==OPTAB.end()){
                error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid mnemonic :"<<inst.mnemonic<<"\n";
                exit(0);
            }
            else if(OPTAB[inst.mnemonic].format==1){
                inst.obj_code = to_hex(OPTAB[inst.mnemonic].opcode,2);
            }
            else if(OPTAB[inst.mnemonic].format==2){
                vector<string> regs = split_comma(inst.operand);
                int r1 = 0, r2 = 0;
                if(regs.size()>=1){
                    if(is_number(regs[0])){
                        r1 = str_to_dec(regs[0]);
                    }
                    else if(REGISTER.find(regs[0])!=REGISTER.end()){
                        r1 = REGISTER[regs[0]].num;
                    }
                    else{
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid register" << regs[0] << "\n";
                        exit(0);
                    }
                }
                if(regs.size()>=2){
                    if(is_number(regs[1])){
                        r2 = str_to_dec(regs[1]);
                    }
                    else if(REGISTER.find(regs[1])!=REGISTER.end()){
                        r2 = REGISTER[regs[1]].num;
                    }
                    else{
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid register" << regs[1] << "\n";
                        exit(0);
                    }
                }
                inst.obj_code = to_hex(OPTAB[inst.mnemonic].opcode,2) + to_hex(r1,1) + to_hex(r2,1);
            }
            else if(OPTAB[inst.mnemonic].format==3){
                if(inst.mnemonic=="RSUB"){
                    inst.obj_code = to_hex(OPTAB[inst.mnemonic].opcode + 3,2) + to_hex(0,4);
                    if(inst.operand!=""){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : RSUB should not have operand: " << inst.operand << "\n";
                        exit(0);
                    }
                }
                else{
                    int n = 0, i = 0, x = 0, b = 0, p = 0, e = 0;
                    int abs_address = 0;
                    bool isnum = false;
                    if(inst.operand[0]=='#'){
                        if(inst.operand.length()>1 && inst.operand.substr(inst.operand.length()-2, 2) == ",X"){
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Both immediate and indexed addressing.\n";
                            exit(0);
                        }
                        n = 0;
                        i = 1;
                        if(is_number(inst.operand.substr(1,inst.operand.length()-1))){
                            isnum = true;
                            p=0;
                            abs_address = str_to_dec(inst.operand.substr(1,inst.operand.length()-1));
                        }
                        else if(csections[_i].sym_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].sym_tab.end()){
                            abs_address = csections[_i].sym_tab[inst.operand.substr(1,inst.operand.length()-1)].address;
                            if(csections[_i].sym_tab[inst.operand.substr(1,inst.operand.length()-1)].absolute){
                                isnum = true;// absolute addressing
                            }
                        }
                        else if(csections[_i].lit_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].lit_tab.end()){
                            abs_address = csections[_i].lit_tab[inst.operand.substr(1,inst.operand.length()-1)].value;
                        }
                        else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].ref.end()){
                            abs_address = 0;
                        }
                        else{
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid operand: " << inst.operand << "\n";
                            exit(0);
                        }   
                    }
                    else if(inst.operand[0]=='@'){
                        if(inst.operand.length()>1 && inst.operand.substr(inst.operand.length()-2, 2) == ",X"){
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Both immediate and indexed addressing.\n";
                            exit(0);
                        }
                        n = 1;
                        i = 0;
                        if(is_number(inst.operand.substr(1,inst.operand.length()-1))){
                            isnum = true;
                            p=0;
                            abs_address = str_to_dec(inst.operand.substr(1,inst.operand.length()-1));
                        }
                        else if(csections[_i].sym_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].sym_tab.end()){
                            abs_address = csections[_i].sym_tab[inst.operand.substr(1,inst.operand.length()-1)].address;
                        }
                        else if(csections[_i].lit_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].lit_tab.end()){
                            abs_address = csections[_i].lit_tab[inst.operand.substr(1,inst.operand.length()-1)].address;
                        }
                        else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].ref.end()){
                            abs_address = 0;
                        }
                        else{
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid operand: " << inst.operand << "\n";
                            exit(0);
                        }
                    }
                    else if(inst.operand.length()>1 && inst.operand.substr(inst.operand.length()-2, 2) == ",X"){
                        x=1;n=1;i=1;
                        if(is_number(inst.operand.substr(0, inst.operand.length() - 2))){
                            isnum = true;
                            p=0;
                            abs_address = str_to_dec(inst.operand.substr(0,inst.operand.length()-2));
                        }
                        else if(csections[_i].sym_tab.find(inst.operand.substr(0,inst.operand.length()-2))!=csections[_i].sym_tab.end()){
                            abs_address = csections[_i].sym_tab[inst.operand.substr(0,inst.operand.length()-2)].address;
                        }
                        else if(csections[_i].lit_tab.find(inst.operand.substr(0,inst.operand.length()-2))!=csections[_i].lit_tab.end()){
                            abs_address = csections[_i].lit_tab[inst.operand.substr(0,inst.operand.length()-2)].address;
                        }
                        else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(0,inst.operand.length()-2))!=csections[_i].ref.end()){
                            abs_address = 0;
                        }
                        else{
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid operand: " << inst.operand << "\n";
                            exit(0);
                        }
                    }
                    else{
                        n = 1;
                        i = 1;
                        x=0;
                        if(is_number(inst.operand)){
                            isnum = true;
                            p=0;
                            abs_address = str_to_dec(inst.operand);
                        }
                        else if(csections[_i].sym_tab.find(inst.operand)!=csections[_i].sym_tab.end()){
                            abs_address = csections[_i].sym_tab[inst.operand].address;
                        }
                        else if(csections[_i].lit_tab.find(inst.operand)!=csections[_i].lit_tab.end()){
                            abs_address = csections[_i].lit_tab[inst.operand].address;
                        }
                        else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand)!=csections[_i].ref.end()){
                            abs_address = 0;
                        }
                        else{
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid operand: " << inst.operand << "\n";
                            exit(0);
                        }
                    }
                    
                    int disp = abs_address - (inst.address + 3);
                    if(disp>=pow(2,11) || disp<(-pow(2,11))){
                        b=1;p=0;e=0;
                    }
                    else{
                        p=1;b=0;e=0;
                    }

                    int relative_address = abs_address;
                    if(b==1){
                        relative_address -= REGISTER["B"].value;
                    }
                    else if(p==1){
                        relative_address -= (inst.address + inst.length);
                    }
                    if(x==1){
                        relative_address -= REGISTER["X"].value;
                    }
                    if(isnum){
                        p=0;b=0;x=0;
                        relative_address = abs_address;
                    }
                    inst.obj_code = to_hex(OPTAB[inst.mnemonic].opcode + 2*n + i,2) + to_hex(x*8 + b*4 + p*2 + e*1, 1) + to_hex(relative_address,3);
                }
            }
            else if(OPTAB[inst.mnemonic].format==4){
                if(inst.mnemonic=="+RSUB"){
                    inst.obj_code = to_hex(OPTAB[inst.mnemonic].opcode + 3,2) + to_hex(0,6);
                    if(inst.operand!=""){
                        error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : RSUB should not have operand: " << inst.operand << "\n";
                        exit(0);
                    }
                }
                else{
                    int n = 0, i = 0, x = 0, b = 0, p = 0, e = 1;
                    int abs_address = 0;
                    bool isnum = false;
                    if(inst.operand[0]=='#'){
                        if(inst.operand.length()>1 && inst.operand.substr(inst.operand.length()-2, 2) == ",X"){
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Both immediate and indexed addressing.\n";
                            exit(0);
                        }
                        n = 0;
                        i = 1;
                        if(is_number(inst.operand.substr(1,inst.operand.length()-1))){
                            isnum = true;
                            p=0;
                            abs_address = str_to_dec(inst.operand.substr(1,inst.operand.length()-1));
                        }
                        else if(csections[_i].sym_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].sym_tab.end()){
                            abs_address = csections[_i].sym_tab[inst.operand.substr(1,inst.operand.length()-1)].address;
                        }
                        else if(csections[_i].lit_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].lit_tab.end()){
                            abs_address = csections[_i].lit_tab[inst.operand.substr(1,inst.operand.length()-1)].address;
                        }
                        else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].ref.end()){
                            abs_address = 0;
                        }
                        else{
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid operand: " << inst.operand << "\n";
                            exit(0);
                        }
                    }
                    else if(inst.operand[0]=='@'){
                        if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].ref.end()){
                            csections[_i].mod_record.push_back(info_mod(inst.address+1, 5, inst.operand.substr(1,inst.operand.length()-1),1));
                        }
                        else{
                            csections[_i].mod_record.push_back(info_mod(inst.address+1, 5, csections[_i].name,1));
                        }
                        if(inst.operand.length()>1 && inst.operand.substr(inst.operand.length()-2, 2) == ",X"){
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Both immediate and indexed addressing.\n";
                            exit(0);
                        }
                        n = 1;
                        i = 0;
                        if(is_number(inst.operand.substr(1,inst.operand.length()-1))){
                            isnum = true;
                            p=0;
                            abs_address = str_to_dec(inst.operand.substr(1,inst.operand.length()-1));
                        }
                        else if(csections[_i].sym_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].sym_tab.end()){
                            abs_address = csections[_i].sym_tab[inst.operand.substr(1,inst.operand.length()-1)].address;
                        }
                        else if(csections[_i].lit_tab.find(inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].lit_tab.end()){
                            abs_address = csections[_i].lit_tab[inst.operand.substr(1,inst.operand.length()-1)].address;
                        }
                        else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(1,inst.operand.length()-1))!=csections[_i].ref.end()){
                            abs_address = 0;
                        }
                        else{
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid operand: " << inst.operand << "\n";
                            exit(0);
                        }
                    }
                    else if(inst.operand.length()>1 && inst.operand.substr(inst.operand.length()-2, 2) == ",X"){
                        if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(0,inst.operand.length()-2))!=csections[_i].ref.end()){
                            csections[_i].mod_record.push_back(info_mod(inst.address+1, 5, inst.operand.substr(0,inst.operand.length()-2),1));
                        }
                        else{
                            csections[_i].mod_record.push_back(info_mod(inst.address+1, 5, csections[_i].name,1));
                        }
                        x=1;n=1;i=1;
                        if(is_number(inst.operand.substr(0, inst.operand.length() - 2))){
                            isnum = true;
                            p=0;
                            abs_address = str_to_dec(inst.operand.substr(0,inst.operand.length()-2));
                        }
                        else if(csections[_i].sym_tab.find(inst.operand.substr(0,inst.operand.length()-2))!=csections[_i].sym_tab.end()){
                            abs_address = csections[_i].sym_tab[inst.operand.substr(0,inst.operand.length()-2)].address;
                        }
                        else if(csections[_i].lit_tab.find(inst.operand.substr(0,inst.operand.length()-2))!=csections[_i].lit_tab.end()){
                            abs_address = csections[_i].lit_tab[inst.operand.substr(0,inst.operand.length()-2)].address;
                        }
                        else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),inst.operand.substr(0,inst.operand.length()-2))!=csections[_i].ref.end()){
                            abs_address = 0;
                        }
                        else{
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid operand: " << inst.operand << "\n";
                            exit(0);
                        }
                    }
                    else{
                        n = 1;
                        i = 1;
                        x=0;p=0;
                        vector<string> params = find_parameters(inst.operand);
                        if(params.back()=="**"){
                            error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Invalid Expression For WORD Statement";
                            exit(0);
                        }
                        int value=0,plus=0,minus=0;
                        bool moddi = false;
                        for(int p=0;p<params.size();p++){
                            if(params[p][0]=='+'){
                                if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))!=csections[_i].ref.end()){
                                    csections[_i].mod_record.push_back(info_mod(inst.address+1,5,params[p].substr(1,params[p].length()-1),1));
                                    moddi = true;
                                }
                                if(params[p].substr(1,params[j].length()-1)=="*"){
                                    plus++;
                                    value += inst.address+inst.length;
                                }
                                else if(is_number(params[p].substr(1,params[p].length()-1))){
                                    value += str_to_dec(params[p].substr(1,params[p].length()-1));
                                }
                                else if(csections[_i].sym_tab.find(params[p].substr(1,params[p].length()-1))!=csections[_i].sym_tab.end()){
                                    value += csections[_i].sym_tab[params[p].substr(1,params[p].length()-1)].address;
                                    if(!csections[_i].sym_tab[params[p].substr(1,params[p].length()-1)].absolute){
                                        plus++;
                                    }
                                }
                                else if(csections[_i].lit_tab.find(params[p].substr(1,params[p].length()-1))!=csections[_i].lit_tab.end()){
                                    value += csections[_i].lit_tab[params[p].substr(1,params[p].length()-1)].address;
                                }
                                else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))==csections[_i].ref.end()){
                                    error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Symbol Not Found : "<<(params[p].substr(1,params[p].length()-1));
                                    exit(0);
                                }
                            }
                            else if(params[p][0]=='-'){
                                if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))!=csections[_i].ref.end()){
                                    csections[_i].mod_record.push_back(info_mod(inst.address+1,5,params[p].substr(1,params[p].length()-1),0));
                                    moddi = true;
                                }
                                if(params[p].substr(1,params[p].length()-1)=="*"){
                                    minus++;
                                    value -= inst.address+inst.length;
                                }
                                else if(is_number(params[p].substr(1,params[p].length()-1))){
                                    value -= str_to_dec(params[p].substr(1,params[p].length()-1));
                                }
                                else if(csections[_i].sym_tab.find(params[p].substr(1,params[p].length()-1))!=csections[_i].sym_tab.end()){
                                    value -= csections[_i].sym_tab[params[p].substr(1,params[p].length()-1)].address;
                                    if(!csections[_i].sym_tab[params[p].substr(1,params[p].length()-1)].absolute){
                                        minus++;
                                    }
                                }
                                else if(csections[_i].lit_tab.find(params[p].substr(1,params[p].length()-1))!=csections[_i].lit_tab.end()){
                                    value -= csections[_i].lit_tab[params[p].substr(1,params[p].length()-1)].address;
                                }
                                else if(std::find(csections[_i].ref.begin(),csections[_i].ref.end(),params[p].substr(1,params[p].length()-1))==csections[_i].ref.end()){
                                    error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Symbol Not Found : "<<(params[p].substr(1,params[p].length()-1));
                                    exit(0);
                                }
                            }
                            
                        }
                        if(!moddi){
                            csections[_i].mod_record.push_back(info_mod(inst.address+1, 5, csections[_i].name,1));
                        }
                        if((plus-minus)!=0 && (plus-minus)!=1){
                            error<<"Line "<<i+1<<" : Invalid Expression.";
                            exit(0);
                        }
                        abs_address = value;
                    }
                    
                    inst.obj_code = to_hex(OPTAB[inst.mnemonic].opcode + 2*n + i,2) + to_hex(x*8 + b*4 + p*2 + e*1, 1) + to_hex(abs_address,5);
                }
            }
            if(inst.obj_code.length()>8){
                error<<"Line "<<j+1<<" in "<<csections[_i].name<<" : Object code too long: " << inst.obj_code << "\n";
                exit(0);
            }
            csections[_i].instructions[j] = inst;
        }
    }

    // write object program
    for(int i=0;i<N;i++){
        if(csections[i].name.length()>6){
            error<<"Section name too long: " << csections[i].name << "\n";
            exit(0);
        }
        output<<"H"<<pad(csections[i].name,6)<<""<<to_hex(csections[i].start,6)<<""<<to_hex(csections[i].end - csections[i].start,6)<<"\n";

        // define records
        if(csections[i].def.size()>0){
            output<<"D";
            for(auto it=csections[i].def.begin();it!=csections[i].def.end();it++){
                if(it->first.length()>6){
                    error<<"Error while writing object program: Symbol name too long: " << it->first << "\n";
                    exit(0);
                }
                output<<pad(it->first,6)<<""<<to_hex(it->second,6)<<"";
            }
            output<<"\n";
        }
        // reference records
        if(csections[i].ref.size()>0){
            output<<"R";
            for(auto it=csections[i].ref.begin();it!=csections[i].ref.end();it++){
                if(it->length()>6){
                    error<<"Error while writing object program: Symbol name too long: " << *it << "\n";
                    exit(0);
                }
                output<<pad(*it,6)<<"";
            }
            output<<"\n";
        }
        int curr_sz = 0;
        int sz=0;
        int _j = 0;
        // text records
        for(int j=0;j<csections[i].instructions.size();j++){
            if(csections[i].instructions[j].obj_code.length()>0){
                curr_sz = csections[i].instructions[j].obj_code.length()/2;
                if((sz + curr_sz) > 30){
                    output<<"T"<<to_hex(csections[i].instructions[_j].address,6)<<""<<to_hex((sz),2)<<"";
                    for(int k=_j;k<j;k++){
                        if(csections[i].instructions[k].obj_code.length()>0)
                            output<<csections[i].instructions[k].obj_code;
                        if(k==j-1)
                            output<<"";
                    }
                    output<<"\n";
                    _j = j;
                    sz = curr_sz;
                }
                else{
                    sz += csections[i].instructions[j].obj_code.length()/2;
                }
            }
            else if(csections[i].instructions[j].mnemonic == "RESW" || csections[i].instructions[j].mnemonic == "RESB"){
                if(sz>0){
                    output<<"T"<<to_hex(csections[i].instructions[_j].address,6)<<""<<to_hex((sz),2)<<"";
                    for(int k=_j;k<j;k++){
                        output<<csections[i].instructions[k].obj_code;
                    }
                    output<<"\n";
                    _j = j+1;
                    sz = 0;
                }
            }
            else if(sz==0){
                _j = j+1;
            }
        }
        if(sz>0){
            output<<"T"<<to_hex(csections[i].instructions[_j].address,6)<<""<<to_hex((sz),2)<<"";
            for(int k=_j;k<csections[i].instructions.size();k++){
                output<<csections[i].instructions[k].obj_code<<"";
            }
            output<<"\n";
        }
        // modification records
        for(int j=0;j<csections[i].mod_record.size();j++){
            output<<"M"<<to_hex(csections[i].mod_record[j].address,6)<<""<<to_hex(csections[i].mod_record[j].length,2)<<""<<((csections[i].mod_record[j].plus)?"+":"-")<<csections[i].mod_record[j].name<<"\n";
        }

        if(i==0)
            output<<"E"<<to_hex(csections[i].start,6)<<"\n\n";
        else
            output<<"E\n\n";
    }
}
