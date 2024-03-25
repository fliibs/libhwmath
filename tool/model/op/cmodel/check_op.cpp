#include "check_op.h"

void checker_op::addVariable(const std::string &variableName,mp::cpp_int &value){
    VariablesTable[variableName]=value;
}

void printVariables(std::unordered_map <std::string, mp::cpp_int> table) {
    for (const auto& pair : table) {
        std::cout << "Variable Name: " <<std::setw(10)<< pair.first << ", Value: " << pair.second << std::endl;
    }
}