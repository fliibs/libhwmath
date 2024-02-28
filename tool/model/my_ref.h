#ifndef MY_REF_H
#define MY_REF_H
#include "ref.h"
template <typename T>
class My_ref:public Ref<T>{
public:
    My_ref(){
        typename Ref<T>::FunctionPointer newFunc = std::bind(&My_ref::min, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        // FunctionPointer newFunc = std::bind(&My_ref::min,this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        // addFunction("min",newFunc);
        this->addFunction("min", newFunc);
    }
    T min(T a, T b,T c){
        T res;
        res = a<=b?a:b;
        return res;
    }
};
#endif