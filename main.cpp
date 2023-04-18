#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<cstdio>
#include<map>
#include<vector>
#include<math.h>
#include <algorithm>

#include "utils.cpp"
#include "file_handler.cpp"
#include "structs.cpp"
#include "tables.cpp"
#include "pass1.cpp"
#include "pass2.cpp"

int main()
{   
    init_tables();
    open_files();
    check_files();
    run_pass1();
    run_pass2();
    close_files();
    return 0;
}