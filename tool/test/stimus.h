#ifndef STIMUS_H
#define STIMUS_H

#include <functional>
#include <unordered_map>
#include <iostream>
#include <random>
#include <limits>
#include <cstdint>
#include "../model/struct.h"
#include "../model/cal.h"

class Stimus {
public:
    //function pointer
    using StimusPointer = std::function<void(FpBase* res)>;

    //random generator
    void get_rand_reg(FpBase* res);
    void get_rand_nan(FpBase* res);
    void get_rand_zero(FpBase* res);
    void get_rand_inf(FpBase* res);

    //
    StimusPointer stimus_func;
    Stimus(std::string table_index){
        auto it = stimus_table.find(table_index);
        if(it == stimus_table.end()){
            it = stimus_table.find("reg");
            stimus_func = it->second;
            std::cout <<"Fail to set "<<std::left<<std::setw(20)<<"Stimus";  
            std::cout << std::left<<std::setw(20)<<",default set is"<<"reg"<<std::endl;  
        }
        else{
            stimus_func = it->second;
        }
    }

    //default
    Stimus();
    
    //table
    std::unordered_map<std::string, StimusPointer> stimus_table = {
        {"reg",  std::bind(&Stimus::get_rand_reg, this ,std::placeholders::_1)},
        {"nan",  std::bind(&Stimus::get_rand_nan, this ,std::placeholders::_1)},
        {"zero", std::bind(&Stimus::get_rand_zero, this,std::placeholders::_1)},
        {"inf",  std::bind(&Stimus::get_rand_inf, this ,std::placeholders::_1)}
        };
};


#endif