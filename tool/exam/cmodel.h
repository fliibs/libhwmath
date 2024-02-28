// cmodel.h
#ifndef cmodel_h
#define cmodel_h
#include "../test/info.h"
class c_lib {
public:
    Info info1;
    int  rnd_mode;
    template<typename T1, typename T2, typename T3, typename T4>
    T4 mul(const T1& a, const T2& b, const T3& c);
};
#include "cmodel_impl.h"  // Include the file with function template definitions
#endif
