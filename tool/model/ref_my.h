#ifndef REF_MY_H
#define REF_MY_H
#include "ref.h"
class My_ref:public Ref{
public:
    IF if1;
    My_ref(){
        typename Ref::Ref_Ptr newFunc = std::bind(&My_ref::min, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        this->addFunction("min", newFunc);
    }
    void min(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res);
};
#endif