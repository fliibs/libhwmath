#ifndef REF_H
#define REF_H
#include <functional>
#include <unordered_map>
#include <iostream>

template <typename T>
class Ref{
public:
    using FunctionPointer = std::function<T(T, T, T)>;
    T add(T a, T b,T c){
        T res;
        res = a+b;
        return res;
    }
    T mul(T a, T b,T c){
        T res;
        res = a*b;
        return res;
    }
    T fma(T a, T b,T c){
        T res;
        res = a*b+c;
        return res;
    }
    void addFunction(const std::string& functionName, FunctionPointer func) {
        functionTable[functionName] = func;
    }
    void removeFunction(const std::string& functionName) {
        functionTable.erase(functionName);
    }
    std::unordered_map<std::string, FunctionPointer> functionTable = {
        {"add", std::bind(&Ref::add, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {"mul", std::bind(&Ref::mul, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)},
        {"fma", std::bind(&Ref::fma, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)}
    };
};
#endif