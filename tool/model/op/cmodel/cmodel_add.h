#ifndef MY_CMODEL_ADD
#define MY_CMODEL_ADD
#include <functional>
#include <unordered_map>
#include <iostream>
#include "check_op.h"

class cmodel_add:public checker_op{
public:
    std::array<int,5> add(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res);
};
#endif