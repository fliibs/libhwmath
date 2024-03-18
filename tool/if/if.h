#ifndef IF_H
#define IF_H
#include "../test/info.h"
#include "../model/struct.h"
template <typename T>
T set_mask(int width){
    T res=0;
    for(int i=0;i<width;i++){
        res = 1+(res<<1);
    }
    return res;
}

class IF{
public:
    Info    info1;
    float   Fp32toFloat(const FpBase& FP_num);
    double  Fp64toDouble(const FpBase& FP_num);
    half    Fp16toHalf(const FpBase& FP_num);
    FpBase  FloattoFp32(const float& flt_num);
    FpBase  DoubletoFp64(const double& flt_num);
    FpBase  HalftoFp16(const half& flt_num);
};
#endif