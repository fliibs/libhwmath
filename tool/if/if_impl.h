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
    // info1.debug=1;
    float res= *(T_flt*)&bits; 
    return res;
}

template <typename T_fp,typename T_flt>
T_fp IF::float2fp(const T_flt&float_num){
    T_fp   res;
    uint64_t bits;
    if(typeid(T_flt).name()==typeid(float).name()){
        bits= *(uint32_t*)&(float_num);
        printf("T_flt is float\n");
    }
    else if(typeid(T_flt).name()==typeid(double).name()){
        bits= *(uint64_t*)&(float_num);
        printf("T_flt is double\n");

    }
    printf("bits is %b\n",bits);
    
    mp::cpp_int mant_max;
    mp::cpp_int expo_max;

    expo_max = get_mant_av_bits(res.expo_w);
    mant_max = get_mant_av_bits(res.mant_w);

    uint64_t expo_max_64=0;
    expo_max_64=static_cast<uint32_t>(expo_max);
    uint64_t mant_max_64=0;
    mant_max_64=static_cast<uint32_t>(mant_max);

    
    res.sign  = bits >> (res.expo_w+ res.mant_w);
    res.expo  = (bits >> res.mant_w) & expo_max_64;
    
    printf("res_expo is %u\n",res.expo );
    uint64_t res_mant   = bits & mant_max_64;
    res.mant = res_mant;
    return res;
}


#endif