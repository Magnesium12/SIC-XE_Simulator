#include<string>
#include<map>
#include<vector>

// #include "structs.cpp"

void init_tables()
{

// Format 1 and 2 instructions----------

OPTAB["ADDR"].opcode=0x90;
OPTAB["ADDR"].format=2;

OPTAB["CLEAR"].opcode=0xb4;
OPTAB["CLEAR"].format=2;

OPTAB["COMPR"].opcode=0xa0;
OPTAB["COMPR"].format=2;

OPTAB["DIVR"].opcode=0x9c;
OPTAB["DIVR"].format=2;

OPTAB["FIX"].opcode=0xc4;
OPTAB["FIX"].format=1;

OPTAB["FLOAT"].opcode=0xc0;
OPTAB["FLOAT"].format=1;

OPTAB["HIO"].opcode=0xf4;
OPTAB["HIO"].format=1;

OPTAB["MULR"].opcode=0x98;
OPTAB["MULR"].format=2;

OPTAB["NORM"].opcode=0xC8;
OPTAB["NORM"].format=1;

OPTAB["RMO"].opcode=0xAC;
OPTAB["RMO"].format=2;

OPTAB["SHIFTL"].opcode=0xA4;
OPTAB["SHIFTL"].format=2;

OPTAB["SHIFTR"].opcode=0xA8;
OPTAB["SHIFTR"].format=2;

OPTAB["SIO"].opcode=0xF0;
OPTAB["SIO"].format=1;

OPTAB["SUBR"].opcode=0x94;
OPTAB["SUBR"].format=2;

OPTAB["SVC"].opcode=0xB0;
OPTAB["SVC"].format=2;

OPTAB["TIO"].opcode=0xF8;
OPTAB["TIO"].format=1;

OPTAB["TIXR"].opcode=0xB8;
OPTAB["TIXR"].format=2;

// Format 3 instructions----------

OPTAB["ADD"].opcode=0x18;
OPTAB["ADD"].format=3;

OPTAB["ADDF"].opcode=0x58;
OPTAB["ADDF"].format=3;

OPTAB["AND"].opcode=0x40;
OPTAB["AND"].format=3;

OPTAB["COMP"].opcode=0x28;
OPTAB["COMP"].format=3;

OPTAB["COMPF"].opcode=0x88;
OPTAB["COMPF"].format=3;

OPTAB["DIV"].opcode=0x24;
OPTAB["DIV"].format=3;

OPTAB["DIVF"].opcode=0x64;
OPTAB["DIVF"].format=3;

OPTAB["J"].opcode=0x3c;
OPTAB["J"].format=3;

OPTAB["JEQ"].opcode=0x30;
OPTAB["JEQ"].format=3;

OPTAB["JGT"].opcode=0x34;
OPTAB["JGT"].format=3;

OPTAB["JLT"].opcode=0x38;
OPTAB["JLT"].format=3;

OPTAB["JSUB"].opcode=0x48;
OPTAB["JSUB"].format=3;

OPTAB["LDA"].opcode=0x00;
OPTAB["LDA"].format=3;

OPTAB["LDB"].opcode=0x68;
OPTAB["LDB"].format=3;

OPTAB["LDCH"].opcode=0x50;
OPTAB["LDCH"].format=3;

OPTAB["LDF"].opcode=0x70;
OPTAB["LDF"].format=3;

OPTAB["LDL"].opcode=0x08;
OPTAB["LDL"].format=3;

OPTAB["LDS"].opcode=0x6C;
OPTAB["LDS"].format=3;

OPTAB["LDT"].opcode=0x74;
OPTAB["LDT"].format=3;

OPTAB["LDX"].opcode=0x04;
OPTAB["LDX"].format=3;

OPTAB["LPS"].opcode=0xD0;
OPTAB["LPS"].format=3;

OPTAB["MUL"].opcode=0x20;
OPTAB["MUL"].format=3;

OPTAB["MULF"].opcode=0x60;
OPTAB["MULF"].format=3;

OPTAB["OR"].opcode=0x44;
OPTAB["OR"].format=3;

OPTAB["RD"].opcode=0xD8;
OPTAB["RD"].format=3;

OPTAB["RSUB"].opcode=0x4C;
OPTAB["RSUB"].format=3;

OPTAB["SSK"].opcode=0xEC;
OPTAB["SSK"].format=3;

OPTAB["STA"].opcode=0x0C;
OPTAB["STA"].format=3;

OPTAB["STB"].opcode=0x78;
OPTAB["STB"].format=3;

OPTAB["STCH"].opcode=0x54;
OPTAB["STCH"].format=3;

OPTAB["STF"].opcode=0x80;
OPTAB["STF"].format=3;

OPTAB["STI"].opcode=0xD4;
OPTAB["STI"].format=3;

OPTAB["STL"].opcode=0x14;
OPTAB["STL"].format=3;

OPTAB["STS"].opcode=0x7C;
OPTAB["STS"].format=3;

OPTAB["STSW"].opcode=0xE8;
OPTAB["STSW"].format=3;

OPTAB["STT"].opcode=0x84;
OPTAB["STT"].format=3;

OPTAB["STX"].opcode=0x10;
OPTAB["STX"].format=3;

OPTAB["SUB"].opcode=0x1C;
OPTAB["SUB"].format=3;

OPTAB["SUBF"].opcode=0x5C;
OPTAB["SUBF"].format=3;

OPTAB["TD"].opcode=0xE0;
OPTAB["TD"].format=3;

OPTAB["TIX"].opcode=0x2C;
OPTAB["TIX"].format=3;

OPTAB["WD"].opcode=0xDC;
OPTAB["WD"].format=3;

// Format 4 instructions----------

OPTAB["+ADD"].opcode=0x18;
OPTAB["+ADD"].format=4;

OPTAB["+ADDF"].opcode=0x58;
OPTAB["+ADDF"].format=4;

OPTAB["+AND"].opcode=0x40;
OPTAB["+AND"].format=4;

OPTAB["+COMP"].opcode=0x28;
OPTAB["+COMP"].format=4;

OPTAB["+COMPF"].opcode=0x88;
OPTAB["+COMPF"].format=4;

OPTAB["+DIV"].opcode=0x24;
OPTAB["+DIV"].format=4;

OPTAB["+DIVF"].opcode=0x64;
OPTAB["+DIVF"].format=4;

OPTAB["+J"].opcode=0x3c;
OPTAB["+J"].format=

OPTAB["+JEQ"].opcode=0x30;
OPTAB["+JEQ"].format=4;

OPTAB["+JGT"].opcode=0x34;
OPTAB["+JGT"].format=4;

OPTAB["+JLT"].opcode=0x38;
OPTAB["+JLT"].format=4;

OPTAB["+JSUB"].opcode=0x48;
OPTAB["+JSUB"].format=4;

OPTAB["+LDA"].opcode=0x00;
OPTAB["+LDA"].format=4;

OPTAB["+LDB"].opcode=0x68;
OPTAB["+LDB"].format=4;

OPTAB["+LDCH"].opcode=0x50;
OPTAB["+LDCH"].format=4;

OPTAB["+LDF"].opcode=0x70;
OPTAB["+LDF"].format=4;

OPTAB["+LDL"].opcode=0x08;
OPTAB["+LDL"].format=4;

OPTAB["+LDS"].opcode=0x6C;
OPTAB["+LDS"].format=4;

OPTAB["+LDT"].opcode=0x74;
OPTAB["+LDT"].format=4;

OPTAB["+LDX"].opcode=0x04;
OPTAB["+LDX"].format=4;

OPTAB["+LPS"].opcode=0xD0;
OPTAB["+LPS"].format=4;

OPTAB["+MUL"].opcode=0x20;
OPTAB["+MUL"].format=4;

OPTAB["+MULF"].opcode=0x60;
OPTAB["+MULF"].format=4;

OPTAB["+OR"].opcode=0x44;
OPTAB["+OR"].format=4;

OPTAB["+RD"].opcode=0xD8;
OPTAB["+RD"].format=4;

OPTAB["+RSUB"].opcode=0x4C;
OPTAB["+RSUB"].format=4;

OPTAB["+SSK"].opcode=0xEC;
OPTAB["+SSK"].format=4;

OPTAB["+STA"].opcode=0x0C;
OPTAB["+STA"].format=4;

OPTAB["+STB"].opcode=0x78;
OPTAB["+STB"].format=4;

OPTAB["+STCH"].opcode=0x54;
OPTAB["+STCH"].format=4;

OPTAB["+STF"].opcode=0x80;
OPTAB["+STF"].format=4;

OPTAB["+STI"].opcode=0xD4;
OPTAB["+STI"].format=4;

OPTAB["+STL"].opcode=0x14;
OPTAB["+STL"].format=4;

OPTAB["+STS"].opcode=0x7C;
OPTAB["+STS"].format=4;

OPTAB["+STSW"].opcode=0xE8;
OPTAB["+STSW"].format=4;

OPTAB["+STT"].opcode=0x84;
OPTAB["+STT"].format=4;

OPTAB["+STX"].opcode=0x10;
OPTAB["+STX"].format=4;

OPTAB["+SUB"].opcode=0x1C;
OPTAB["+SUB"].format=4;

OPTAB["+SUBF"].opcode=0x5C;
OPTAB["+SUBF"].format=4;


OPTAB["+TD"].opcode=0xE0;
OPTAB["+TD"].format=4;

OPTAB["+TIX"].opcode=0x2C;
OPTAB["+TIX"].format=4;

OPTAB["+WD"].opcode=0xDC;
OPTAB["+WD"].format=4;

// Assembler directives--------------------------

OPTAB["WORD"].opcode=0;
OPTAB["WORD"].format=0;

OPTAB["RESW"].opcode=0;
OPTAB["RESW"].format=0;

OPTAB["RESB"].opcode=0;
OPTAB["RESB"].format=0;

OPTAB["BYTE"].opcode=0;
OPTAB["BYTE"].format=0;

OPTAB["BASE"].opcode=0;
OPTAB["BASE"].format=0;

OPTAB["NOBASE"].opcode=0;
OPTAB["NOBASE"].format=0;

OPTAB["EQU"].opcode=0;
OPTAB["EQU"].format=0;

OPTAB["ORG"].opcode=0;
OPTAB["ORG"].format=0;

OPTAB["START"].opcode=0;
OPTAB["START"].format=0;


OPTAB["END"].opcode=0;
OPTAB["END"].format=0;

OPTAB["LTORG"].opcode=0;
OPTAB["LTORG"].format=0;


OPTAB["CSECT"].opcode=0;
OPTAB["CSECT"].format=0;


OPTAB["EXTDEF"].opcode=0;
OPTAB["EXTDEF"].format=0;

OPTAB["EXTREF"].opcode=0;
OPTAB["EXTREF"].format=0;

OPTAB["USE"].opcode=0;
OPTAB["USE"].format=0;


//--------------------------------


// REGISTERS'-INFO----------------

REGISTER["A"].num=0;

REGISTER["X"].num=1;

REGISTER["L"].num=2;

REGISTER["B"].num=3;

REGISTER["S"].num=4;

REGISTER["T"].num=5;

REGISTER["F"].num=6;

REGISTER["PC"].num=8;

REGISTER["SW"].num=9;

//--------------------------------

}


