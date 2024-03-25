#ifndef MY_CMODEL_MUL
#define MY_CMODEL_MUL
#include <functional>
#include <unordered_map>
#include <iostream>
#include "check_op.h"
#define echo(variable)                     \
        {                                   \
            mp::cpp_int value = variable;   \
            value = variable;          \
            addVariable(std::string(#variable), value); \
        }
class cmodel_mul:public checker_op{
public:
    std::array<int,5> mul(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res);
};

#endif