#ifndef struct_h
#define struct_h
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;
struct Fp32 {
    bool sign;
    uint32_t expo;
    uint32_t mant;
    static const int expo_w = 8;
    static const int mant_w = 23;
};
#endif