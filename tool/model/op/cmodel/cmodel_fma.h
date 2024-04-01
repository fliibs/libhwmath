#ifndef MY_CMODEL_FMA
#define MY_CMODEL_FMA
#include <functional>
#include <unordered_map>
#include <iostream>
#include "check_op.h"

struct Fp_classify{
    bool a_is_0        ;
    bool b_is_0        ;
    bool c_is_0        ;
    bool a_is_nan      ;
    bool b_is_nan      ;
    bool c_is_nan      ;
    bool a_is_inf      ;
    bool b_is_inf      ;
    bool c_is_inf      ; 
    bool a_expo_is_0   ;
    bool b_expo_is_0   ;
    bool c_expo_is_0   ;

};


struct Mul_res{
    uint32_t    expo_mul;
    mp::cpp_int mant_mul;
    bool        sign_mul;
};

struct Sft_Inv_res{
    bool        sub       ; 
    uint32_t    dif       ; //the difference betw c-(a+b)
    bool        dif_sign  ;
    uint32_t    expo_str  ; //store the correct expo_nums.for the sake of shifter,10 bits
    uint32_t    shift_num ;
    mp::cpp_int c_mant_l  ;
    mp::cpp_int c_mant_s  ;
    mp::cpp_int c_inv     ;
};

class cmodel_fma:public checker_op{
public:
    Fp_classify       fpclass1;

    std::array<int,5> fma(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res);
    void              classify(const FpBase& a, const FpBase& b, const FpBase& c);
    Mul_res           fma_mul(const FpBase& a , const FpBase& b, const FpBase& res);
    Sft_Inv_res       fma_sft_inv(Mul_res mul_res, const FpBase& c);
};
#endif