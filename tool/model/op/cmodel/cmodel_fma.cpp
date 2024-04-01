#include "cmodel_fma.h"

std::array<int,5> cmodel_fma::fma(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res){
    std::array<int,5> status;
    //---------
    //classify
    //---------
    classify(a,b,c);

    //-----------
    //Mul a and b
    //-----------
    info1.debug_printf("--------------------------");
    info1.debug_printf("------Mul a and b---------");
    info1.debug_printf("--------------------------");
    Mul_res mul_res1=fma_mul(a,b,*res);

    //------------------------ 
    //shift and inversion of a 
    //------------------------
    info1.debug_printf("----------------------------------------");
    info1.debug_printf("-------shift and inversion of a---------");
    info1.debug_printf("----------------------------------------");


    (*res).sign = a.sign;
    (*res).expo = a.expo;
    (*res).mant = a.mant;
    return status ;
}

void cmodel_fma::classify(const FpBase& a, const FpBase& b,  const FpBase& c){
    bool a_expo_is_zero = (a.expo==0)   ;
    bool b_expo_is_zero = (b.expo==0)   ;
    bool c_expo_is_zero = (c.expo==0)   ;

    uint32_t a_expo_max;
    uint32_t b_expo_max;
    uint32_t c_expo_max;

    a_expo_max          = set_expo_max(a.expo_w);
    b_expo_max          = set_expo_max(b.expo_w);
    c_expo_max          = set_expo_max(c.expo_w);

    bool a_expo_is_ff   = (a.expo==a_expo_max) ;
    bool b_expo_is_ff   = (b.expo==a_expo_max) ;
    bool c_expo_is_ff   = (c.expo==a_expo_max) ;
    bool a_mant_is_00   = (a.mant==0)          ;
    bool b_mant_is_00   = (b.mant==0)          ;
    bool c_mant_is_00   = (c.mant==0)          ;


    fpclass1.a_is_0         = (a_expo_is_zero && a_mant_is_00)      ;
    fpclass1.b_is_0         = (b_expo_is_zero && b_mant_is_00)      ;
    fpclass1.c_is_0         = (c_expo_is_zero && c_mant_is_00)      ;
    fpclass1.a_is_nan       = (a_expo_is_ff   && (!a_mant_is_00))   ;
    fpclass1.b_is_nan       = (b_expo_is_ff   && (!b_mant_is_00))   ;
    fpclass1.c_is_nan       = (c_expo_is_ff   && (!c_mant_is_00))   ;
    fpclass1.a_is_inf       = (a_expo_is_ff   && a_mant_is_00)      ;
    fpclass1.b_is_inf       = (b_expo_is_ff   && b_mant_is_00)      ;
    fpclass1.c_is_inf       = (c_expo_is_ff   && c_mant_is_00)      ;

    fpclass1.a_expo_is_0    = (a.expo==0)                           ;
    fpclass1.b_expo_is_0    = (b.expo==0)                           ;
    fpclass1.c_expo_is_0    = (c.expo==0)                           ;
}

Mul_res cmodel_fma::fma_mul(const FpBase& a, const FpBase& b, const FpBase& res){
    Mul_res res_mul;
    //mant
    uint64_t a_mant_in;
    uint64_t b_mant_in;
    a_mant_in   = ((1-fpclass1.a_expo_is_0)<<a.mant_w)+a.mant;
    b_mant_in   = ((1-fpclass1.b_expo_is_0)<<b.mant_w)+b.mant;
    res_mul.mant_mul= a_mant_in * b_mant_in;
    
    //expo
    uint32_t a_expo_max   = set_expo_max(a.expo_w);
    uint32_t b_expo_max   = set_expo_max(b.expo_w);
    uint32_t res_expo_max = set_expo_max(res.expo_w);

    res_mul.expo_mul  = a.expo + static_cast<uint32_t>(fpclass1.a_expo_is_0) - (a_expo_max/2) 
                        + b.expo + static_cast<uint32_t>(fpclass1.b_expo_is_0) - (b_expo_max/2) 
                        +(res_expo_max/2); 

    res_mul.sign_mul  = a.sign ^ b.sign;
    return res_mul;
}

Sft_Inv_res cmodel_fma::fma_sft_inv(Mul_res mul_res, const FpBase& c){
    //sub
    Sft_Inv_res sft_inv_res1;
    sft_inv_res1.sub = mul_res.sign_mul ^ c.sign;
    
    //dif :: c-(a+b)
    mp::cpp_int c_mant_in;
    uint32_t c_expo_max;
    c_expo_max      = set_expo_max(c.expo_w);
    c_mant_in       = ((1-fpclass1.c_expo_is_0)<<c.mant_w) + c.mant;
    sft_inv_res1.dif= c.expo + fpclass1.c_expo_is_0 - mul_res.expo_mul;
    
    //dif_sign
    uint32_t expo_sign;
    expo_sign               = get_expo_sign_bit(c.expo_w);
    sft_inv_res1.dif_sign   = sft_inv_res1.dif & expo_sign;

    //expo
    uint32_t shift_num;//
    uint32_t shift_num_0;//when dif>0
    uint32_t shift_num_1;//when dif<0
    
    

    return sft_inv_res1;
}