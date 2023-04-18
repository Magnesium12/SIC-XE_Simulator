#include<string>
#include<map>
#include<vector>

using namespace std;

//stores info of register (value,exist)
struct info_reg
{
    int num;
    int value;
};

//stores info of literal (address,value,block,exist)
struct info_literal
{
    int address;// address assigned to the operand
    int value;// operand value
    int length; // length of literal
};

//stores info of opcode (value,format,exist)
struct info_op
{
    int opcode;
    int format;
};

//stores info of symbol (address,absolute,exist)
struct info_sym
{
    int address;
    bool absolute;
    info_sym(){
        this->absolute=false;
    }
    info_sym(int val)
    {
        this->absolute = false;
        this->address = val;
    }
};

//stores info of modification record (address,length,name,plus)
struct info_mod
{
    int address;
    int length;
    string name;
    bool plus;
    info_mod()
    {
        this->name = "";
    }
    info_mod(int address, int length, string nname, bool plus)
    {
        this->address = address;
        this->length = length;
        this->name = nname;
        this->plus = plus;
    }
};


struct instruction{
    int address;    //locctr
    string label;
    string mnemonic;
    string operand;
    string obj_code;
    int length;

    instruction(){
        this->address=-0x1;
        this->label="";
        this->mnemonic="";
        this->operand="";
        this->obj_code="";
        this->length=0;
    }

    instruction(string label,string mnemonic,string operand){
        this->address=-0x1;
        this->label=label;
        this->mnemonic=mnemonic;
        this->operand=operand;
        this->obj_code="";
        this->length=0;
    }
};

struct csect
{
    string name;
    int start;
    int end;
    bool valid_base;
    map<string,int> def;
    vector<string> ref;
    map<string, info_literal> lit_tab;
	vector<info_mod> mod_record;   // MODIFICATION RECORD
    vector<instruction> instructions;
	map<string, info_sym> sym_tab;
    csect(){
        this->valid_base=0;
    }
    csect(string nname, int init_locctr){
        this->start=init_locctr;
        this->name=nname;
        this->valid_base=0;
    }
};

std::vector<csect> csections;

//This maps the instructions(mnemonic codes) with thier Opcode and their format
std::map<std::string,info_op> OPTAB;

//This maps the register mnemonic with their number
std::map<std::string,info_reg> REGISTER;