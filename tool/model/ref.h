#ifndef REF_H
#define REF_H
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../if/if.h"
#include <fenv.h>

#define SET_RND_MODE(num) \
    switch (num) { \
        case 1: \
            fesetround(FE_DOWNWARD); \
            break; \
        case 2: \
            fesetround(FE_UPWARD); \
            break; \
        case 3: \
            fesetround(FE_TONEAREST); \
            break; \
        case 0: \
            fesetround(FE_TOWARDZERO); \
            break; \
    } \

template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res, typename T_flt>
class Ref{
public:
    IF if1;  
    using Ref_Ptr = std::function<T_FP_res(T_FP_a, T_FP_b, T_FP_c, int)>;
    T_FP_res add(T_FP_a a, T_FP_b b,T_FP_c c,int rnd_mode){
        SET_RND_MODE(rnd_mode);
        T_flt     a_flt  = if1.fp2float<T_FP_a,T_flt>(a);
        T_flt     b_flt  = if1.fp2float<T_FP_b,T_flt>(b);
        T_flt     res_flt= a_flt+b_flt;
        T_FP_res  res    = if1.float2fp<T_FP_res,T_flt>(res_flt);
        return res;
    }
    T_FP_res mul(T_FP_a a, T_FP_b b,T_FP_c c,int rnd_mode){
        SET_RND_MODE(rnd_mode);
        T_flt     a_flt  = if1.fp2float<T_FP_a,T_flt>(a);
        T_flt     b_flt  = if1.fp2float<T_FP_b,T_flt>(b);
        T_flt     res_flt= a_flt*b_flt;
        T_FP_res  res    = if1.float2fp<T_FP_res,T_flt>(res_flt);
        return res;
    }
    T_FP_res fma(T_FP_a a, T_FP_b b,T_FP_c c,int rnd_mode){
        SET_RND_MODE(rnd_mode);
        T_flt     a_flt  = if1.fp2float<T_FP_a,T_flt>(a);
        T_flt     b_flt  = if1.fp2float<T_FP_b,T_flt>(b);
        T_flt     c_flt  = if1.fp2float<T_FP_c,T_flt>(c);
        T_flt     res_flt= a_flt*b_flt+c_flt;
        T_FP_res  res    = if1.float2fp<T_FP_res,T_flt>(res_flt);
        return res;
    }
    void addFunction(const std::string& functionName, Ref_Ptr func) {
        functionTable[functionName] = func;
    }
    void removeFunction(const std::string& functionName) {
        functionTable.erase(functionName);
    }
    std::unordered_map<std::string, Ref_Ptr> functionTable = {
        {"add"            ,std::bind(&Ref::add, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)},
        {"mul"            ,std::bind(&Ref::mul, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)},
        {"fma"            ,std::bind(&Ref::fma, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)},
        {"defaultfunction",std::bind(&Ref::add, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)}
    };
};
#endif