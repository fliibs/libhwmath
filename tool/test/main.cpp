// #include "checker.h"
// #include "../model/struct.h"
#include <cassert>
// #include "stimus.h"
// int main(int argc,char* argv[]){
//     Checker<Fp32> checker1;
//     Fp32 a;
//     Fp32 b;
//     a.sign = 0;
//     a.expo = 255;
//     a.mant = 15649;
//     b.sign = 0;
//     b.expo = 255;
//     b.mant = 15649;
//     // checker1.assert_T(a,b);
//     // int x=0;

//     // bool fail;
//     // if(a==b)
//     //     fail=0;
//     // else
//     //     fail=1;
//     // printf("fail is %d\n",fail);

//     bool res;
//     if(argc>=2){
//         std::string functionName = argv[1];
//         auto it = checker1.functionTable.find(functionName);
//         if (it != checker1.functionTable.end()) {
//             res=it->second(a,b); // 调用函数
//             printf("result is %d\n",res);
//         } else {
//             std::cout << "Function not found\n";
//         }
//     } else {
//         std::cout << "Usage: " << argv[0] << " <function_name>\n";
//     }
//     return 0;
// }

//test stimus
int main(){
    Stimus stimus1;
    std::string in1="nan";
    auto it = stimus1.stimus_table.find(in1);
    Fp32 res;
    res=it->second();
    // res = stimus1.get_rand_nan();
    printf("sign is %d\n",res.sign);
    printf("expo is %b\n",res.expo);
    printf("mant is %b\n",res.mant);
    return 0;
}
// #include <iostream>
// #include <unordered_map>

#include <iostream>
#include <unordered_map>
#include <functional>
#include "stimus.h"

int main() {
    Stimus<Fp32> stimus_instance;
    // 调用函数指针
    Fp32 result;

    for(int i = 0;i<10;i++){
        result = stimus_instance.stimus_table["inf"]();
        printf("%d============\n",i);
        printf("res::sign is %d\n",result.sign);
        printf("res::expo is %b\n",result.expo);
        printf("ref::expo is %b\n",0xff);
        printf("res::mant is %b\n",result.mant);
        printf("ref::mant is %b\n",0x7fffff);
        printf("==============\n",i);

    }
    // std::cout << "Result: " << result << std::endl;
    return 0;
}
