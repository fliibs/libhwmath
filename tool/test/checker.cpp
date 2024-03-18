#include "checker.h"
bool Checker::assert_T(const FpBase& input1, const FpBase& input2){
    bool fail=0;
    if(input1==input2){
        fail=0;
    }
    else{
        std::cerr << "assertion failed"<<std::endl;
        fail=1;
    }
    return fail;
}