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
    mp::cpp_int c_mant_inv;
    mp::cpp_int c_mant_in ;
};

struct Sign_det_res{
    bool complement   ;
    bool sign_add     ;
};

struct Csa_add_in{
    mp::cpp_int c_mant_s_l  ;
    mp::cpp_int mul_mant_l  ;
};

struct LOA_res{
    uint32_t    zero_nums_0;
    uint32_t    zero_nums_1;
    mp::cpp_int mask_cpl   ;
};

#define BIT_CAL(res_vector,cal_eq) {   \
    std::vector<int> out(add_width);   \
    mp::cpp_int a_in   ;   \
    mp::cpp_int b_in   ;   \
    int a_lst          ;   \  
    int b_lst          ;   \
    a_in   = csa_add_in.c_mant_s_l ;    \
    b_in   = csa_add_in.mul_mant_l ;    \
    for(int i=0;i<add_width;i++){       \
        a_lst  = static_cast<int>(a_in & 0x01);    \
        b_lst  = static_cast<int>(b_in & 0x01);    \
        out[i] = cal_eq       ;       \
        std::cout<<out[i]<<std::endl; \
        a_in   = a_in >> 1    ;   \
        b_in   = b_in >> 1    ;   \
    }   \
    res_vector = out    ;      \   
}

class cmodel_fma:public checker_op{
public:
    Fp_classify       fpclass1;
    
    //fma 
    std::array<int,5> fma(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res);

    void              classify(const FpBase& a, const FpBase& b, const FpBase& c);
    Mul_res           fma_mul(const FpBase& a , const FpBase& b, const FpBase& res);
    Sft_Inv_res       fma_sft_inv(Mul_res mul_res, const FpBase& c, int a_mant_w, int b_mant_w);
    bool              fma_get_st1(Sft_Inv_res sft_inv_in);
    Sign_det_res      fma_sign_det(Mul_res mul_res_in,Sft_Inv_res sft_inv_in,int expo_w_dif,bool c_sign);

    mp::cpp_int       fma_csa_add(Csa_add_in csa_add_in,int mul_mant_w,int c_mant_w);
    mp::cpp_int       fma_cas_add_cpl(Csa_add_in csa_add_in,int mul_mant_w,int c_mant_w);
    
    LOA_res           fma_loa(Csa_add_in csa_add_in,int add_width);
    uint32_t          fma_loa_sel(uint32_t zero_nums_1_uc,mp::cpp_int mask_long); //to get the currect loa

    //cal used only here
    bool              cal_shift_out(uint32_t shift_num,mp::cpp_int c_mant_s);
    template <typename T>
    T                 cal_shift_r_with_sign(T input, int width, int shift_num){
        T output            ;   
        T sign_top_bit      ;
        T shift_sign_lst    ;
        T sign_top          ;
        
        sign_top_bit  = 1 << width                      ;
        sign_top      = (sign_top_bit & input)? 1:0     ;
        shift_sign_lst= sign_top<<(width-shift_num+1)   ;

        output        = 0                             ;
        for(int i = 0;i<shift_num;i++){
            output    = (output  << 1)+shift_sign_lst ; 
        }
        output = output + (input>>shift_num);
        return output;
    }
};  
#endif