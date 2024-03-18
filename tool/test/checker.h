#ifndef CHECKER_H
#define CHECKER_H
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../model/struct.h"
class Checker{
public:
    bool assert_T(const FpBase& input1, const FpBase& input2);
    using FunctionPointer = std::function<bool(const FpBase&,const FpBase&)>;
    std::unordered_map<std::string, FunctionPointer> functionTable = {
        {"assert", std::bind(&Checker::assert_T, this, std::placeholders::_1, std::placeholders::_2)},
    };
};

#endif