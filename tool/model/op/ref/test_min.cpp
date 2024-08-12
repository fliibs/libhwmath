// #include <functional>
// #include <unordered_map>
// #include <iostream>
// #include <half.hpp>
// #include "/home/lgzhu/dev/try_min/tool/if/if.h"
// #include <fenv.h>
// #include "/home/lgzhu/dev/try_min/tool/model/struct.h"
// #include "/home/lgzhu/dev/try_min/tool/model/op/ref/ref_op.h"
// #include "/home/lgzhu/dev/try_min/tool/model/op/ref/ref_min.h"

// int main() {
//     std::feclearexcept(FE_ALL_EXCEPT);
//     feclearexcept(FE_ALL_EXCEPT);
//     // 示例使用自定义格式表示两个浮点数
//     FpBase a;
//     FpBase b;
//     FpBase* res;
//     std::array<int,5> arr={0,0,0,0,0};

//     a.sign = 0;
//     a.expo = 31;
//     a.mant = 0;
//     b.sign = 1;
//     b.expo = 5;
//     b.mant = 266;
//     // 比较两个浮点数的大小
//     half a_h;
//     half b_h;
//     IF if1;

//     a_h=convert2flt<half>(a);
//     b_h=convert2flt<half>(b);
//     std::cout << "valuea a_h is: "<< a_h <<std::endl;
//     std::cout << "valuea b_h is: "<< b_h <<std::endl;
//     *res = if1.HalftoFp16(std::min(a_h, b_h));

//     arr_excps
//     // std::cout << "valuea res1 is: "<< static_cast<int>(res1) <<std::endl;
    
//     for (int num : arr) {
//         // 循环遍历当前整数的每一位
//         for (int i = 31; i >= 0; --i) { // 32位整数
//             // 检查当前位是否为1
//             int bit = (num >> i) & 1;
//             // 打印当前位
//             std::cout << bit;
//         }
//         std::cout << std::endl;
//     }
    
    

//     return 0;
// }

