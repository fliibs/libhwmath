#ifndef REF_H
#define REF_H
#include "./op/ref/ref_add.h"
#include "./op/ref/ref_mul.h"
#include "./op/ref/ref_fma.h"
#include "./op/ref/ref_min.h"

class ref{
public:
    //function pointer
    using Ref_Ptr = std::function<std::array<int,5>(const FpBase&, const FpBase&, const FpBase&, const int&, FpBase*)>;

    //op
    ref_add ref_add1;
    ref_mul ref_mul1;
    ref_fma ref_fma1;
    ref_min ref_min1;

    //table
    std::unordered_map<std::string, Ref_Ptr> functionTable = {
        {"add" ,std::bind(&ref_add::add, &ref_add1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
        {"mul" ,std::bind(&ref_mul::mul, &ref_mul1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
        {"fma" ,std::bind(&ref_fma::fma, &ref_fma1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
        {"min" ,std::bind(&ref_min::min, &ref_min1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
    };
    
    //default
    ref(){}
    
    //
    Ref_Ptr ref_func;
    ref(std::string table_index){
        auto it = functionTable.find(table_index);
        if(it == functionTable.end()){
            it = functionTable.find("reg");
            ref_func = it->second;
            std::cout <<"Fail to set "<<std::left<<std::setw(20)<<"Ref";  
            std::cout << std::left<<std::setw(20)<<",default set is"<<"mul"<<std::endl;  
        }
        else{
            ref_func = it->second;
        }
    }


};
#endif