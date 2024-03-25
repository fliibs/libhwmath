//cmodel.h
#ifndef cmodel_h
#define cmodel_h
#include <functional>
#include <unordered_map>
#include <iostream>
#include "cal.h"
#include "./op/cmodel/cmodel_add.h"
#include "./op/cmodel/cmodel_fma.h"
#include "./op/cmodel/cmodel_mul.h"

class cmodel{        
    public:
        using C_Ptr = std::function<std::array<int,5>(const FpBase&,const FpBase&,const FpBase&,const int&,FpBase*)>;

        //initialize handle
        cmodel_add c_add;
        cmodel_mul c_mul;
        cmodel_fma c_fma;
        
        //add handle here
        //cmodel_min c_min;


        std::unordered_map<std::string, C_Ptr> functionTable = {
            {"add", std::bind(&cmodel_add::add, &c_add, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
            {"mul", std::bind(&cmodel_mul::mul, &c_mul, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
            {"fma", std::bind(&cmodel_fma::fma, &c_fma, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
            //add handle here
            // {"min", std::bind(&cmodel_min::min, &c_min, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
        };

        std::unordered_map<std::string, std::map<std::string, mp::cpp_int>*> VariablesTable = {
            {"add",&c_add.VariablesTable},
            {"mul",&c_mul.VariablesTable},
            {"fma",&c_fma.VariablesTable},
            //add handle here
            // {"min",&c_min.VariablesTable},
        };

        std::unordered_map<std::string, Info*> InfoTable ={
            {"add",&c_add.info1},
            {"mul",&c_mul.info1},
            {"fma",&c_fma.info1},
            //add handle here
            // {"min",&c_min.info1},
        };

};

#endif