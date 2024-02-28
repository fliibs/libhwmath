//main.cpp
#include "struct.h"
#include "my_ref.h"

int main(int argc, char* argv[]){
    My_ref My_ref1;
    float a = 3.0;
    float b = 2.0;
    float c = 10.0;
    float res;
    if (argc >= 2) {
        std::string functionName = argv[1];
        // 查找函数并调用
        auto it = My_ref1.functionTable.find(functionName);
        if (it != My_ref1.functionTable.end()) {
            res=it->second(a,b,c); // 调用函数
            printf("result is %f\n",res);
        } else {
            std::cout << "Function not found\n";
        }
    } else {
        std::cout << "Usage: " << argv[0] << " <function_name>\n";
    }
    return 0;
}