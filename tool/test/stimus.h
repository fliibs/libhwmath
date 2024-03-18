#ifndef STIMUS_H
#define STIMUS_H

#include <functional>
#include <unordered_map>
#include <iostream>
#include <random>
#include <limits>
#include <cstdint>
#include "../model/struct.h"
#include "../model/cal.h"

class Stimus {
public:
    using FunctionPointer = std::function<void(FpBase* res)>;

    void get_rand_reg(FpBase* res);
    void get_rand_nan(FpBase* res);
    void get_rand_zero(FpBase* res);
    void get_rand_inf(FpBase* res);

    std::unordered_map<std::string, FunctionPointer> stimus_table = {
        {"reg",  std::bind(&Stimus::get_rand_reg, this ,std::placeholders::_1)},
        {"nan",  std::bind(&Stimus::get_rand_nan, this ,std::placeholders::_1)},
        {"zero", std::bind(&Stimus::get_rand_zero, this,std::placeholders::_1)},
        {"inf",  std::bind(&Stimus::get_rand_inf, this ,std::placeholders::_1)}
    };
};


#endif