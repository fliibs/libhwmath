#ifndef MY_CMODEL_FMA
#define MY_CMODEL_FMA
#include <functional>
#include <unordered_map>
#include <iostream>
#include "check_op.h"

class cmodel_fma:public checker_op{
public:
    std::array<int,5> fma(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res);
};
#endif