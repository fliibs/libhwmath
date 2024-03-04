#ifndef CHECKER_H
#define CHECKER_H
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../model/struct.h"
template<typename T>
class Checker{
public:
    bool assert_T(const T& input1, const T& input2){
        bool fail=0;
        if(input1==input2){
            fail=0;
        }
        else{
            std::cerr << "assertion failed"<<std::endl;
            fail=1;
        }
        return fail;
    }
    using FunctionPointer = std::function<bool(const T&,const T&)>;
    std::unordered_map<std::string, FunctionPointer> functionTable = {
        {"assert", std::bind(&Checker::assert_T, this, std::placeholders::_1, std::placeholders::_2)},
    };
};

#endif