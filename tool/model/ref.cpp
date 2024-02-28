#include "ref.h"
float Ref::add(float a, float b,float c){
    float result;
    result = a+b;
    return result;
}

float Ref::mul(float a, float b,float c){
    float result;
    result = a*b;
    return result;
}

float Ref::fma(float a, float b,float c){
    float result;
    result = a*b+c;
    return result;
}