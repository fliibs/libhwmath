#ifndef CHECKER_H
#define CHECKER_H
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../model/struct.h"
template<typename T>
class Checker{
public:
    bool assert_T(const T& input1, const T& input2){
        bool fail=0;
        if(input1==input2){
            fail=0;
        }
        else{
            std::cerr << "assertion failed"<<std::endl;
            printf("input one is ---------------\n");
            std::cout<<"sign : "<<input1.sign<<std::endl;
            std::cout<<"expo : "<<input1.expo<<std::endl;
            std::cout<<"mant : "<<input1.mant<<std::endl;
            printf("----------------------------\n");
            printf("input two is ---------------\n");
            std::cout<<"sign : "<<input2.sign<<std::endl;
            std::cout<<"expo : "<<input2.expo<<std::endl;
            std::cout<<"mant : "<<input2.mant<<std::endl;
            printf("----------------------------\n");
            fail=1;
            std::terminate();
        }
        return fail;
    }
    using FunctionPointer = std::function<bool(const T&,const T&)>;
    std::unordered_map<std::string, FunctionPointer> functionTable = {
        {"assert", std::bind(&Checker::assert_T, this, std::placeholders::_1, std::placeholders::_2)},
    };
};

#endif