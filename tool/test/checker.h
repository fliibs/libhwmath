#ifndef CHECKER_H
#define CHECKER_H
#include <functional>
#include <unordered_map>
#include <map>
#include <iostream>
#include "../model/struct.h"
#include "../wrapper/arg_passer.h"
#define status_check(status_name,num)   \
    if(arr1[num]!=arr2[num])    { \
        std::cout<<status_name<<" check fail"<<std::endl; \
        printf("in arr1[%d] is %d\n",num,arr1[num]);    \
        printf("in arr2[%d] is %d\n",num,arr2[num]);    \
    }
    
class Checker{
public:
    //arg
    Arg arg_in;
    //Function Pointer
    using FunctionPointer = std::function<bool(const FpBase&,const FpBase& ,const std::array<int,5>,const std::array<int,5>)>;

    //func
    bool assert_T(const FpBase& input1, const FpBase& input2, const std::array<int,5> arr1,const std::array<int,5> arr2);

    //table
    std::unordered_map<std::string, FunctionPointer> functionTable = {
        {"assert", std::bind(&Checker::assert_T, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)},
    };
    void printVariables(std::map<std::string, mp::cpp_int> table);
    void compareVariables(std::map <std::string, mp::cpp_int> table1,std::map <std::string, mp::cpp_int> table2);

    //
    FunctionPointer checker_func;
    // Checker(std::string table_index){
    //         auto it_func = functionTable.find(table_index);
    //         if(it_func == functionTable.end()){
    //             it_func = functionTable.find("assert");
    //             checker_func = it_func->second;
    //             std::cout <<"Fail to set "<<std::left<<std::setw(20)<<"Checker";  
    //             std::cout << std::left<<std::setw(20)<<",default set is"<<"assert"<<std::endl;  
    //         }
    //         else{
    //             checker_func = it_func->second;
    //         }
    // }
    Checker(Arg arg1):
    arg_in(arg1){
        auto it_func = functionTable.find(arg1.checker[0]);
        if(it_func == functionTable.end()){
            it_func = functionTable.find("assert");
            checker_func = it_func->second;
            std::cout <<"Fail to set "<<std::left<<std::setw(20)<<"Checker";  
            std::cout << std::left<<std::setw(20)<<",default set is"<<"assert"<<std::endl;  
        }
        else{
            checker_func = it_func->second;
        }
    }
    //
    Checker(){}
};

#endif