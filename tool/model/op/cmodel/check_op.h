#ifndef check_op_h
#define check_op_h
#include <iostream>
#include "../../../test/info.h"
#include <unordered_map>
#include <map>
#include <iomanip>
#include "../../struct.h"
#include "../../cal.h"

#define echo(variable)                     \
        {                                   \
            mp::cpp_int value = variable;   \
            value = variable;          \
            VariablesTable[std::string(#variable)]=value;   \
        }
        
class checker_op{
public:
    Info info1;
    std::map<std::string, mp::cpp_int> VariablesTable;
    void addVariable(const std::string &variableName,mp::cpp_int &value);
};

void printVariables(std::map <std::string, mp::cpp_int> table);

#endif