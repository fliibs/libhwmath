#ifndef reflib_h
#define reflib_h
#include "fp32.h"

class Pointer {
public:
    typedef float (Pointer::*Function_Ref)(float, float, float);
    typedef Fp32 (Fp32::*Function_C)(const Fp32&, const Fp32&, const Fp32&);
    bool debug_c;
    uint32_t round_mode;
    Pointer();
    float mul(float num1, float num2, float num3);
    float add(float num1, float num2, float num3);
    float fma(float num1, float num2, float num3);
    float res_op(float num1, float num2, float num3);
    Fp32 c_op(Fp32 num1, Fp32 num2, Fp32 num3);
    void setMulOperation();
    void setAddOperation();
    void setFMAOperation();
    
private:
    Fp32 fp32;
    Function_Ref function_Ref;
    Function_C function_C;
};

#endif // reflib_h
