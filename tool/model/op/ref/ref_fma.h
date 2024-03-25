#ifndef REF_FMA_H
#define REF_FMA_H
#include "ref_op.h"
class ref_fma:public ref_op{
public:
    // IF if1;
    // My_ref(){
    //     typename Ref::Ref_Ptr newFunc = std::bind(&My_ref::min, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
    //     this->addFunction("min", newFunc);
    // }
    // std::array<int,5> min(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res);
    std::array<int,5> fma(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res);
};
#endif