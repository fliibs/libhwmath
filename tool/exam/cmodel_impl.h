// cmodel_impl.h
#ifndef cmodel_impl_h
#define cmodel_impl_h

#include <iostream>

template<typename T1, typename T2, typename T3, typename T4>
T4 c_lib::mul(const T1& a, const T2& b, const T3& c) {
    T4 result;
    result = a * b;
    // std::cout<<"result is -----"<<std::endl;
    info1.debug_printf("result is----");
    return result;
}

#endif
