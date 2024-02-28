// Ref.h
#ifndef REF_H
#define REF_H

#include <functional>
#include <unordered_map>
#include <iostream>

using FunctionPointer = std::function<void()>;

class Ref {
public:
    void function1(std::string one,std::string two) {
        std::cout << "Function 1\n"<<one<<two<<std::endl;
    }

    void function2(std::string one,std::string two) {
        std::cout << "Function 2\n"<<one<<two<<std::endl;
    }

    void function3(std::string one,std::string two,std::string three) {
        std::cout << "Function 3\n"<<one<<two<<std::endl<<three;
    }

    void addFunction(const std::string& functionName, FunctionPointer func) {
        functionTable[functionName] = func;
    }

    void removeFunction(const std::string& functionName) {
        functionTable.erase(functionName);
    }
    std::unordered_map<std::string, FunctionPointer> functionTable = {
        {"function1", std::bind(&Ref::function1, this)},
        {"function2", std::bind(&Ref::function2, this)},
        {"function3", std::bind(&Ref::function3, this)}
    };
};

#endif  // REF_H
