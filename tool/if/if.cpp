#include "if.h"

float IF::Fp32toFloat(const FpBase& FP_num){
    uint32_t bits=0;
    bits |= FP_num.sign <<31;
    bits |= FP_num.expo <<23;     
    bits |= FP_num.mant;        
    float res=*(float*)&bits;
    return res; 
}

double IF::Fp64toDouble(const FpBase& FP_num){
    uint64_t bits=0;
    uint64_t in_sign=0;
    uint64_t in_expo=0;
    in_sign =static_cast<uint64_t>(FP_num.sign);
    in_expo =static_cast<uint64_t>(FP_num.expo);
    bits |= in_sign <<63;
    bits |= in_expo <<52;     
    bits |= FP_num.mant;   
    double res;     
    memcpy(&res, &bits, sizeof(double));
    return res; 
}

half IF::Fp16toHalf(const FpBase& FP_num){
    uint16 bits=0;
    bits |= FP_num.sign <<15;
    bits |= FP_num.expo <<10;     
    bits |= FP_num.mant;   
    half res=*(half*)&bits;
    return res;
}

FpBase IF::FloattoFp32(const float& flt_num){
    FpBase res;
    uint32_t bits=*(uint32_t*)&flt_num;
    res.expo_w=8;
    res.mant_w=23;
    uint32_t expo_mask;
    uint32_t mant_mask;
    expo_mask = set_mask<uint32_t>(res.expo_w);
    mant_mask = set_mask<uint32_t>(res.mant_w);
    res.sign = bits>>(res.expo_w + res.mant_w);
    res.expo = (bits >> res.mant_w)& expo_mask;
    res.mant = bits & mant_mask;
    return res;
}

FpBase IF::DoubletoFp64(const double&flt_num){
    FpBase res;
    uint64_t bits=*(uint64_t*)&flt_num;
    res.expo_w=11;
    res.mant_w=52;
    uint64_t expo_mask;
    uint64_t mant_mask;
    expo_mask = set_mask<uint64_t>(res.expo_w);
    mant_mask = set_mask<uint64_t>(res.mant_w);
    res.sign = bits>>(res.expo_w + res.mant_w);
    res.expo = (bits >> res.mant_w)& expo_mask;
    res.mant = bits & mant_mask;
    return res;
}

FpBase IF::HalftoFp16(const half&flt_num){
    FpBase res;
    uint16 bits=*(uint16*)&flt_num;
    res.expo_w=5;
    res.mant_w=10;
    uint16 expo_mask;
    uint16 mant_mask;
    expo_mask = set_mask<uint16>(res.expo_w);
    mant_mask = set_mask<uint16>(res.mant_w);
    res.sign = bits>>(res.expo_w + res.mant_w);
    res.expo = (bits >> res.mant_w)& expo_mask;
    res.mant = bits & mant_mask;
    return res;
}
