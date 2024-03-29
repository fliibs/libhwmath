#ifndef CHECKER_H
#define CHECKER_H
#include <functional>
#include <unordered_map>
#include <map>
#include <iostream>
#include "../model/struct.h"

#define status_check(status_name,num)   \
    if(arr1[num]!=arr2[num])    { \
        std::cout<<status_name<<" check fail"<<std::endl; \
        printf("in arr1 is %d\n",arr1[num]);    \
        printf("in arr2 is %d\n",arr2[num]);    \
    }
    
class Checker{
public:
    bool assert_T(const FpBase& input1, const FpBase& input2, const std::array<int,5> arr1,const std::array<int,5> arr2);
    using FunctionPointer = std::function<bool(const FpBase&,const FpBase& ,const std::array<int,5>,const std::array<int,5>)>;
    std::unordered_map<std::string, FunctionPointer> functionTable = {
        {"assert", std::bind(&Checker::assert_T, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)},
    };
    //check variables
    void printVariables(std::map<std::string, mp::cpp_int> table);
    void compareVariables(std::map <std::string, mp::cpp_int> table1,std::map <std::string, mp::cpp_int> table2);
    

};

#endif