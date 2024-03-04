#ifndef MY_REF_H
#define MY_REF_H
#include "ref.h"
template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res, typename T_flt>
class My_ref:public Ref<T_FP_a,T_FP_b,T_FP_c,T_FP_res,T_flt>{
public:
    IF if1;
    My_ref(){
        typename Ref<T_FP_a,T_FP_b,T_FP_c,T_FP_res,T_flt>::Ref_Ptr newFunc = std::bind(&My_ref::min, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        this->addFunction("min", newFunc);
    }
    T_FP_res min(T_FP_a a, T_FP_b b,T_FP_c c,int rnd_mode){
        SET_RND_MODE(rnd_mode); 
        T_flt     a_flt  = if1.fp2float<T_FP_a,T_flt>(a);
        T_flt     b_flt  = if1.fp2float<T_FP_b,T_flt>(b);
        T_flt     res_flt= a_flt<=b_flt?a_flt:b_flt;
        T_FP_res  res    = if1.float2fp<T_FP_res,T_flt>(res_flt);
        return res;
    }
};
#endif