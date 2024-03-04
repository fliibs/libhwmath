#ifndef MY_CMODEL_H
#define MY_CMODEL_H
#include "cmodel.h"
template<typename T1,typename T2,typename T3,typename T4>
class My_cmodel:public Cmodel<T1,T2,T3,T4>{
public:
    My_cmodel(){
        typename Cmodel<T1,T2,T3,T4>::C_Ptr newFunc = std::bind(&My_cmodel::min, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        this->addFunction("min",newFunc);
    }
    T4 min(T1 a, T2 b, T3 c, int rnd_mode){
        T4 res;
        printf("into min\n");
        return res;

    } 
};

#endif