#ifndef REF_OP_H
#define REF_OP_H
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../../../if/if.h"
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
    } 

#define arr_excps   \
    arr[4]=std::fetestexcept(FE_INVALID)  ?1:0; \
    arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; \
    arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; \
    arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; \
    arr[0]=std::fetestexcept(FE_INEXACT)  ?1:0; 

#define half_arr_excps \
    arr[4]=fetestexcept(FE_INVALID)  ?1:0; \
    arr[3]=fetestexcept(FE_DIVBYZERO)?1:0; \
    arr[2]=fetestexcept(FE_OVERFLOW) ?1:0; \
    arr[1]=fetestexcept(FE_UNDERFLOW)?1:0; \
    arr[0]=fetestexcept(FE_INEXACT)  ?1:0; 

class ref_op{
public:
    IF if1;  
    // using Ref_Ptr = std::function<std::array<int,5>(const FpBase&, const FpBase&, const FpBase&, const int&, FpBase*)>;
   
    template <typename T>
    T convert2flt(const FpBase& a){
        T res;
        if(a.expo_w==5){
            half res_1=if1.Fp16toHalf(a);
            res=res_1;
        }
        else if(a.expo_w==8){
            float res_2=if1.Fp32toFloat(a);
            res=res_2;
        }
        else if(a.expo_w==11){
            double res_3;
            res_3=if1.Fp64toDouble(a);
            res=res_3;
        }
        else{
            printf("intput is not among Fpbase");
            std::terminate();
        }
        return res;
    }
};
#endif