#ifndef RTL_MAP_H
#define RTL_MAP_H
#include <unordered_map>
#include <string>

std::unordered_map<std::string,std::string> rtl_map{ //first string g++ argv; second string rtl argv
    {"mul"," +MUL"},
    {"add"," +ADD"},
    {"fma"," +FMA"},
};
#endif