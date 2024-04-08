#ifndef MY_CMODEL_MIN
#define MY_CMODEL_MIN
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
class cmodel_min:public checker_op{
public:

    std::array<int,5> min(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res);
};



#endif