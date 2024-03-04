#ifndef IF_H
#define IF_H
#include "../model/cal.h"
#include "../test/info.h"
class IF{
public:
    Info info1;
    Fp32 floatTOFp32(const float&a){
        Fp32 res;
        uint32_t bits = *(uint32_t*)&a; // get the binary representation of the float
        info1.print_cpp_int(bits,"b");
        res.sign = bits >> 31;          // get the sign bit
        res.expo = bits >> 23 & 0xff; // get the exponent bit
        res.mant = bits & 0x7fffff;     // get the mantissa bit
        printf("res.sign is %d\n",res.sign);
        printf("res.expo is %d\n",res.expo);
        printf("res.mant is %d\n",res.mant);
        return res;
    }
    float fp32tofloat(const Fp32& a){
        uint32_t bits=0;
        bits |= a.sign << 31;         // set the sign bit
        bits |= a.expo << 23;     // set the exponent bit
        bits |= a.mant;           // set the mantissa bit
        float x = *(float*)&bits;   // get the float value
        // printf("a.sign is %d\n",a.sign);
        // printf("a.expo is %d\n",a.expo);
        // printf("a.mant is %d\n",a.mant);
        info1.print_cpp_int(bits,"b");
        return x;
    }
    template <typename T_fp,typename T_flt>
    T_flt fp2float(const T_fp& fp_num);

    template <typename T_fp,typename T_flt>
    T_fp float2fp(const T_flt&float_num);
     
    // template <typename T1,typename T2>
    // T2 int2fp(const T1 & int_num);
    // template <typename T1,typename T2>
    // T2 fp2int(const T1 & fp_num);

};
#include "if_impl.h"

#endif