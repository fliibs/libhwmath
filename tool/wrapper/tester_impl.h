#ifndef TESTER_IMPL_H
#define TESTER_IMPL_H
#include <string>
#include "../model/struct.h"
#include "../test/stimus.h"
#include "tester.h"
#include <fenv.h>
#include "../model/my_ref.h"
#include "../if/if.h"
#include "../model/my_cmodel.h"
#include "../test/checker.h"
#include "../model/rtl_map.h"
#include "../if/pipe.h"
template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res, typename T_flt>
bool Tester<T_FP_a,T_FP_b,T_FP_c,T_FP_res,T_flt>::run(int argc, char **argv){
//-----------------------------
//class func and env init or set
//-----------------------------
////////////////
//get stimus fun
////////////////
    Stimus<T_FP_a> stimu1;
    Stimus<T_FP_b> stimu2;
    Stimus<T_FP_c> stimu3;

    std::string in1_stimuls = argc_string(argc,argv,"in1=");
    std::string in2_stimuls = argc_string(argc,argv,"in2=");
    std::string in3_stimuls = argc_string(argc,argv,"in3=");
    auto in1_func=stimu1.stimus_table.find(in1_stimuls);
    auto in2_func=stimu2.stimus_table.find(in2_stimuls);
    auto in3_func=stimu3.stimus_table.find(in3_stimuls);

////////////////
//set test_times 
////////////////
    int test_times;
    test_times = argc_int(argc,argv,"test_times=");

////////////////
//set ref model
////////////////
    T_FP_a a;
    T_FP_b b;
    T_FP_c c;
    a.sign = 0;
    a.expo = 132;
    a.mant = 100;
    b.sign = 1; 
    b.expo = 154;
    b.mant = 100;

    int rnd_mode=0;
    My_ref<T_FP_a,T_FP_b,T_FP_c,T_FP_res,T_flt> my_ref1;
    std::string ref_string;
    auto ref_func = my_ref1.functionTable.end();
    for(int i=0;i<argc;i++){
        ref_string = argv[i];
        ref_func = my_ref1.functionTable.find(ref_string);
        if(ref_func != my_ref1.functionTable.end())
        {
            printf("ref_func is %s\n",ref_string.c_str());
            break;
        }
        else{
            ref_func = my_ref1.functionTable.find("mul");
            printf("default ref_func is mul\n");
        }
    }

    printf("ref_string is %s\n",ref_string.c_str());

    // printf("res is %f\n",res);

////////////////
//set cmodel
////////////////
    My_cmodel<T_FP_a,T_FP_b,T_FP_c,T_FP_res> my_cmodel1;
    std::string cmodel_string;
    
    auto c_func = my_cmodel1.functionTable.end();
    for(int i=0;i<argc;i++){
        cmodel_string = argv[i];
        c_func = my_cmodel1.functionTable.find(cmodel_string);
        if(c_func != my_cmodel1.functionTable.end())
        {
            printf("c_func is %s\n",cmodel_string.c_str());
            break;
        }
        else{
            c_func = my_cmodel1.functionTable.find("mul");
            printf("default c_func is mul\n");

        }
    }
    printf("cmodel_string is %s\n",cmodel_string.c_str());

////////////////
//set rtl_model
////////////////
    // std::string simv_executable=SIMV_EXECUTABLE_PATH;
    std::string simv_executable="/home/liuyunqi/ymyu/libhwmath/build_sv/./simv";
    std::string quiet   = " -q";
    std::string debug   = " +RTL_DEBUG";
    std::string command;
    if(argc_bool(argc,argv,"DEBUG_RTL"))
        command = simv_executable + quiet +debug;
    else
        command = simv_executable + quiet;
    
    std::string rtl_model;
    for (int i = 0; i < argc; ++i) {
        std::string model_argv = argv[i];
        auto iter = rtl_map.find(model_argv);
        if (iter != rtl_map.end()) {
            rtl_model = iter->second;  // Assign the value to rtl_model
            break;
        } else {
            rtl_model = rtl_map["mul"];  // Assign the default value "mul"
        }
    }

    command = command + rtl_model;
    std::cout << command << std::endl;

    Pipe pipe1(command);
    auto rtl_func=&Pipe::verif_inout<T_FP_a,T_FP_b,T_FP_c,T_FP_res>;


////////////////
//set checker model
////////////////
    Checker<T_FP_res> checker1;
    std::string checker_string;
    auto checker_func = checker1.functionTable.end();
    for(int i=0;i<argc;i++){
        checker_string = argv[i];
        checker_func = checker1.functionTable.find(checker_string);
        if(checker_func != checker1.functionTable.end())
        {
            break;
        }
        else{ 
            checker_func = checker1.functionTable.find("assert");
            printf("default checker_func is checker_func\n");
        }
    }

//////////////////////////
// choose two comparision
//////////////////////////
using mdltype= std::function<T_FP_res(T_FP_a,T_FP_b,T_FP_c,int)>;
mdltype checker_1_func;
mdltype checker_2_func;

//use a table to handle the inputs
std::unordered_map<std::string,mdltype> modeltable;
modeltable["ref"]=(ref_func->second);
modeltable["cmodel"]=(c_func->second);
modeltable["rtl"]=std::bind(rtl_func,&pipe1,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);


std::string checker_1_str=argc_string(argc,argv,"checker1=");
std::string checker_2_str=argc_string(argc,argv,"checker2=");

printf("checker_1_str is %s\n",checker_1_str.c_str());
printf("checker_2_str is %s\n",checker_2_str.c_str());

auto checker_1 = modeltable.find(checker_1_str);
auto checker_2 = modeltable.find(checker_2_str);

if(checker_1 != modeltable.end())
{
    checker_1_func = checker_1->second;
}
else{
    checker_1_func = modeltable["ref"];
    printf("into default check1:ref");
}

if(checker_2 != modeltable.end())
    {
        checker_2_func = checker_2->second;
    }
    else{
        checker_1_func = modeltable["cmodel"];
        printf("into default check2:cmodel");
    }


////////////////
// start test
////////////////
    T_FP_a in_a;
    T_FP_b in_b;
    T_FP_c in_c;

    // in_a.sign=0;
    // in_a.expo=112;
    // in_a.mant=5765334;

    // in_b.sign=0;
    // in_b.expo=14;
    // in_b.mant=7511560;
    T_FP_res out_1;
    T_FP_res out_2;
    bool fail=0;

    for(int i=0;i<test_times;i++){
        in_a=in1_func->second();
        in_b=in2_func->second();
        in_c=in3_func->second();
        out_1=checker_1_func(in_a,in_b,in_c,rnd_mode);
        out_2=checker_2_func(in_a,in_b,in_c,rnd_mode);
        // out_1.print();
        // out_2.print();
        fail =checker_func->second(out_1,out_2);
        if(fail){
            printf("in_a:");
            in_a.print();
            printf("in_b:");
            in_b.print();
            printf("in_c:");
            in_c.print();
            printf("output one is ---------------\n");
            out_1.print();
            printf("----------------------------\n");
            printf("output two is ---------------\n");  
            out_2.print();
            printf("----------------------------\n");
            std::terminate();
        }
    }
    return fail;
}

template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res, typename T_flt>
bool Tester<T_FP_a,T_FP_b,T_FP_c,T_FP_res,T_flt>::argc_bool(int argc, char **argv, const std::string&prefix){
    bool value = false;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0){
            value=true;
        }
    }
    return value;
}

template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res, typename T_flt>
std::string Tester<T_FP_a,T_FP_b,T_FP_c,T_FP_res,T_flt>::argc_string(int argc,char **argv,const std::string&prefix){
    std::string res="zero";
    for (int i =0 ; i<argc ; i++){
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0)
            res = arg.substr(prefix.size());
    }
    return res;
}

template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res, typename T_flt>
int Tester<T_FP_a,T_FP_b,T_FP_c,T_FP_res,T_flt>::argc_int(int argc,char **argv,const std::string&prefix){
    int res_int=0;
    std::string value_str;
    for (int i =0 ; i<argc ; i++){
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0)
            value_str = arg.substr(prefix.size());
            std::istringstream(value_str) >> res_int;
    }
    return res_int;
}
#endif