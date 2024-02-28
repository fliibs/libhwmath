#ifndef IF_IMPL_H
#define IF_IMPL_H
#include "../model/struct.h"
#include "../model/cal.h"
#include <type_traits>

template <typename T_fp,typename T_flt>
T_flt IF::fp2float(const T_fp& fp_num){
    mp::cpp_int bits;
    bits |=static_cast<mp::cpp_int>(fp_num.sign)<<(fp_num.expo_w+fp_num.mant_w);
    bits |=static_cast<mp::cpp_int>(fp_num.expo)<<fp_num.mant_w;
    bits |=static_cast<mp::cpp_int>(fp_num.mant);
    float res= *(T_flt*)&bits; 
    return res;
}


template <typename T_fp,typename T_flt>
T_fp IF::float2fp(const T_flt&float_num){
    T_fp   res;
    double db_float  = float_num;
    uint64_t bits    = *(uint64_t*)&db_float;

    int expo_s;
    int mant_s;
    if(std::is_same<decltype(res.mant),uint64_t>::value){
        expo_s  =  res.expo_w;
        mant_s  =  res.mant_w;
    }else{
        expo_s  = 11;
        mant_s  = 52;
    }
    mp::cpp_int mant_max;
    mp::cpp_int expo_max;

    expo_max = get_mant_av_bits(expo_s);
    mant_max = get_mant_av_bits(mant_s);

    uint64_t expo_max_64;
    expo_max_64=static_cast<uint64_t>(expo_max);
    uint64_t mant_max_64;
    mant_max_64=static_cast<uint64_t>(mant_max);

    bool res_sign       = bits >> (expo_s+ mant_s);
    uint32_t res_expo   = (bits >> mant_s) & expo_max_64;
    uint64_t res_mant   = bits & mant_max_64;

    res.expo = res_expo - static_cast<uint32_t>(std::pow(2,expo_s-1)-1) + static_cast<uint32_t>(std::pow(2,res.expo_w-1)-1);
    res.sign = res_sign;
    res.mant = res_mant>>(mant_s - res.mant_w);
    return res;
}
#endif