//cmodel.h
#ifndef cmodel_h
#define cmodel_h
#include <functional>
#include <unordered_map>
#include <iostream>
#include "../test/info.h"
#include "cal.h"

template<typename T1,typename T2,typename T3,typename T4>
class Cmodel{        
    public:
        using C_Ptr = std::function<T4(T1,T2,T3,int)>;
        Info info1;
        T4 mul(const T1& a,const T2& b,const T3& c,const int& rnd_mode);
        T4 add(const T1& a,const T2& b,const T3& c,const int& rnd_mode);

        void addFunction(const std::string& functionName, C_Ptr func) {
            functionTable[functionName] = func;
        }

        void removeFunction(const std::string& functionName) {
            functionTable.erase(functionName);
        }

        std::unordered_map<std::string, C_Ptr> functionTable = {
            {"add", std::bind(&Cmodel::add, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)},
            {"mul", std::bind(&Cmodel::mul, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)}
        };

};

#include "cmodel_impl.h" // Include the file with function template definitions

#endif