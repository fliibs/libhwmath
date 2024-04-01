#ifndef TESTER_H
#define TESTER_H
#include <string>
#include "../model/struct.h"
#include "../test/stimus.h"
#include "tester.h"
#include <fenv.h>
#include "../model/ref.h"
#include "../model/cmodel.h"
#include "../if/if.h"
#include "../test/checker.h"
// #include "../model/rtl_map.h"
#include "../if/pipe.h"
#include "arg_passer.h"
#include "../model/rtl_model.h"
#include "../test/stimus.h"

class Tester{
public:    
    arg_passer arg_passer1;
    Arg        arg1;

    //test corner
    Stimus     stimu1;
    Stimus     stimu2;
    Stimus     stimu3;

    //test_times
    int        test_times;

    //fptype 
    FpBase     in_a;
    FpBase     in_b;
    FpBase     in_c;
    FpBase     out_1;
    FpBase     out_2;

    //rnd_mode
    int        rnd_mode;

    //op_mode
    ref        my_ref1;   
    cmodel     my_cmodel1;
    rtl_model  my_rtl1;

    //checker
    Checker   checker1;
    using mdltype= std::function<std::array<int,5>(const FpBase& , const FpBase& , const FpBase&, const int&, FpBase *)>;
    mdltype checker_1_func;
    mdltype checker_2_func;
    std::unordered_map<std::string,mdltype> modeltable={
        {"ref"   ,my_ref1.ref_func},
        {"cmodel",my_cmodel1.cmodel_func},
        {"rtl"   ,my_rtl1.rtl_func},
    };

    //debug 
    bool vari_debug;
    int  c_debug;

    Tester(int argc, char **argv):
        arg1        (arg_passer1.set_arg(argc, argv)),
        stimu1      (arg1.corner[0])                 ,
        stimu2      (arg1.corner[1])                 ,
        stimu3      (arg1.corner[2])                 ,
        in_a        (arg1.type[0])                   ,
        in_b        (arg1.type[1])                   ,
        in_c        (arg1.type[2])                   ,
        out_1       (arg1.type[3])                   ,
        out_2       (arg1.type[3])                   ,
        test_times  (arg1.test_times)                ,  
        rnd_mode    (arg1.rnd_mode)                  ,
        //op
        my_ref1     (arg1.op)                        ,
        my_cmodel1  (arg1.op,arg1.debug[0])          ,
        my_rtl1     (arg1.debug[1])                  ,
        //checker
        checker1    (arg1)                           ,
        //debug
        c_debug     (arg1.debug[0])                    
    {
        auto it_checker1 = modeltable.find(arg1.checker[1]);
        auto it_checker2 = modeltable.find(arg1.checker[2]);
        if(it_checker1 == modeltable.end()){
                it_checker1 = modeltable.find("ref");
                checker_1_func = it_checker1->second;
                std::cout <<"Fail to set "<<std::left<<std::setw(20)<<"checker1";  
                std::cout << std::left<<std::setw(20)<<",default set is"<<"ref"<<std::endl;  
            }
        else{
                checker_1_func = it_checker1->second;
            }
        if(it_checker2 == modeltable.end()){
                it_checker2 = modeltable.find("cmodel");
                checker_2_func = it_checker2->second;
                std::cout <<"Fail to set "<<std::left<<std::setw(20)<<"checker2";  
                std::cout << std::left<<std::setw(20)<<",default set is"<<"cmodel"<<std::endl;  
            }
        else{
                checker_2_func = it_checker2->second;
            }
        vari_debug = ((arg1.checker[1]=="cmodel")&&(arg1.checker[2]=="rtl"))||((arg1.checker[2]=="rtl")&&(arg1.checker[1]=="cmodel"));
    }
    //func
    bool  run();
};

#endif 