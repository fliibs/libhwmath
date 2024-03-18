//cmodel.h
#ifndef cmodel_h
#define cmodel_h
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../test/info.h"
#include "cal.h"

class Cmodel{        
    public:
        using C_Ptr = std::function<void(const FpBase&,const FpBase&,const FpBase&,const int&,FpBase*)>;
        Info info1;
        void mul(const FpBase& a,const FpBase& b,const FpBase& c,const int& rnd_mode,FpBase *result);
        void add(const FpBase& a,const FpBase& b,const FpBase& c,const int& rnd_mode,FpBase *result);

        void addFunction(const std::string& functionName, C_Ptr func);
        void removeFunction(const std::string& functionName);
        std::unordered_map<std::string, C_Ptr> functionTable = {
            {"add", std::bind(&Cmodel::add, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)},
            {"mul", std::bind(&Cmodel::mul, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)}
        };

};


#endif