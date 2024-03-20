#ifndef MY_CMODEL_H
#define MY_CMODEL_H
#include "cmodel.h"
class My_cmodel:public Cmodel{
public:
    My_cmodel(){
        typename Cmodel::C_Ptr newFunc = std::bind(&My_cmodel::min, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        this->addFunction("min",newFunc);
    }
    std::array<int,5> min(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res);
};

#endif