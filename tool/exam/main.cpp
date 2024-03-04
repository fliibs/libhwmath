
#include <iostream>
#include <unordered_map>
#include <functional>
// #include "my_ref.h"
int main(int argc, char* argv[]) {
    // ChildRef my_ref_1;
    // if (argc >= 2) {
    //     std::string functionName = argv[1];
    //     // 查找函数并调用
    //     auto it = my_ref_1.functionTable.find(functionName);
    //     if (it != my_ref_1.functionTable.end()) {
    //         it->second(); // 调用函数
    //     } else {
    //         std::cout << "Function not found\n";
    //     }
    // } else {
    //     std::cout << "Usage: " << argv[0] << " <function_name>\n";
    // }
    float a=134197128.000000;
    double b=-6161830486545459984540.000000;
    float c;
    c= a * b;
    double d;
    d= a *b;
    printf("c is %f\n",c);
    printf("d is %f\n",d);

    return 0;
}

// #include <iostream>
// #include <cstdint>
// // #include "struct.h"
// #include "../model/struct.h"
// // struct Fp32 {
// //     bool sign;
// //     uint32_t expo;
// //     uint32_t mant;
// //     static const int expo_w = 8;
// //     static const int mant_w = 23;
// // };

// class IF{
// public:
//     Fp32 floatTOFp32(float a){
//         Fp32 res;
//         uint32_t bits = *(uint32_t*)&a; // get the binary representation of the float
//         res.sign = bits >> 31;          // get the sign bit
//         res.expo = bits >> 23 & 0xff; // get the exponent bit
//         res.mant = bits & 0x7fffff;     // get the mantissa bit
//         return res;
//     }
// };

// int main() {
//     float res1 = 123.456;
//     Fp32 a_old;
//     Fp32 a_new;
//     IF if1;
//     a_old = if1.floatTOFp32(res1);
//     // a_new = if1.float2fp(res1);
//     // 打印 Fp32 结构体的内容
//     std::cout << "Sign: " << a_old.sign << std::endl;
//     std::cout << "Exponent: " << a_old.expo << std::endl;
//     std::cout << "Mantissa: " << a_old.mant << std::endl;
//     std::cout << "Sign: "     << a_new.sign << std::endl;
//     std::cout << "Exponent: " << a_new.expo << std::endl;
//     std::cout << "Mantissa: " << a_new.mant << std::endl;
//     return 0;
// }
