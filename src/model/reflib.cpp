#include "reflib.h"

Pointer::Pointer() {
    function_Ref = &Pointer::mul;
    function_C = &Fp32::fma;
}

float Pointer::mul(float num1, float num2, float num3) {
    // Implementation of mul
    float  res=num1*num2;
    return res;
}

float Pointer::add(float num1, float num2, float num3) {
    // Implementation of add
    float  res=num1+num2;
    return res;
}

float Pointer::fma(float num1, float num2, float num3) {
    // Implementation of fma
    float  res=num1*num2 + num3;
    return res;
}

float Pointer::res_op(float num1, float num2, float num3) {
    float res = (this->*function_Ref)(num1, num2, num3);
    return res;
}

Fp32 Pointer::c_op(Fp32 num1, Fp32 num2, Fp32 num3) {
    fp32.D1.debug   = debug_c;
    fp32.round_mode = round_mode;
    Fp32 res = (fp32.*function_C)(num1, num2, num3);
    return res;
}

void Pointer::setMulOperation() {
    function_Ref = &Pointer::mul;
    function_C = &Fp32::mul;
}

void Pointer::setAddOperation() {
    function_Ref = &Pointer::add;
    function_C = &Fp32::add;
}

void Pointer::setFMAOperation() {
    function_Ref = &Pointer::fma;
    function_C = &Fp32::fma;
}
