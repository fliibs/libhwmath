#ifndef TESTER_IMPL_H
#define TESTER_IMPL_H
#include <string>
#include "../model/struct.h"
#include "../test/stimus.h"
#include "tester.h"
#include <fenv.h>
#include "../model/ref.h"
#include "../model/cmodel.h"
#include "../if/if.h"
#include "../test/checker.h"
#include "../model/rtl_map.h"
#include "../if/pipe.h"
#include "tester.h"

bool Tester::run(int argc, char **argv){
//-----------------------------
//class func and env init or set
//-----------------------------
////////////////
//get stimus fun
////////////////
    Stimus stimu1;
    Stimus stimu2;
    Stimus stimu3;
    std::string fail_string ="fail";
    std::string all_corner=argc_string(argc,argv,"corner_all=");
    auto in1_func=stimu1.stimus_table.end();
    auto in2_func=stimu2.stimus_table.end();
    auto in3_func=stimu3.stimus_table.end();
    if(fail_string!=all_corner){
        in1_func=stimu1.stimus_table.find(all_corner);
        in2_func=stimu2.stimus_table.find(all_corner);
        in3_func=stimu3.stimus_table.find(all_corner);
        //info
        if(in1_func!=stimu1.stimus_table.end() && in2_func!=stimu2.stimus_table.end() && in3_func!=stimu3.stimus_table.end())
            printf("SUCCESSFULLY set all_corner:: %s\n",all_corner.c_str());  
        else{
            in1_func=stimu1.stimus_table.find("zero");
            in2_func=stimu2.stimus_table.find("zero");
            in3_func=stimu3.stimus_table.find("zero");
            printf("FAIL TO set all_corner,default all_corner is zero\n");
        }
    }
    else{
        std::string in1_stimuls = argc_string(argc,argv,"corner1=");
        std::string in2_stimuls = argc_string(argc,argv,"corner2=");
        std::string in3_stimuls = argc_string(argc,argv,"corner3=");
        in1_func=stimu1.stimus_table.find(in1_stimuls);
        in2_func=stimu2.stimus_table.find(in2_stimuls);
        in3_func=stimu3.stimus_table.find(in3_stimuls);
        //info
        if(in1_func!=stimu1.stimus_table.end()){
            printf("SUCCESSFULLY set in1_stimuls:: %s\n",in1_stimuls.c_str());  
        }
        else
        {
            in1_func=stimu1.stimus_table.find("zero");
            printf("FAIL TO set in1_stimuls,default all in1_stimuls is zero\n");
        }

        if(in2_func!=stimu2.stimus_table.end()){
            printf("SUCCESSFULLY set in2_stimuls:: %s\n",in2_stimuls.c_str());  
        }
        else
        {
            in2_func=stimu2.stimus_table.find("zero");
            printf("FAIL TO set in2_stimuls,default all in2_stimuls is zero\n");
        }

        if(in3_func!=stimu3.stimus_table.end()){
            printf("SUCCESSFULLY set in2_stimuls:: %s\n",in2_stimuls.c_str());  
        }
        else
        {
            in2_func=stimu2.stimus_table.find("zero");
            printf("FAIL TO set in3_stimuls,default all in3_stimuls is zero\n");
        }
    }

////////////////
//set test_times 
////////////////
    
    int test_times;
    test_times = argc_int(argc,argv,"test_times=");
    printf("SUCCESSFULLY SET test_times to %d\n",test_times);

//////////////////////////////////
//set input types and output types
//////////////////////////////////
    FpBase in_a;
    FpBase in_b;
    FpBase in_c;
    FpBase out_1;
    FpBase out_2;
    std::string all_type=argc_string(argc,argv,"type_all=");
    if(fail_string!=all_type)
    {
        set_type(all_type,&in_a);
        set_type(all_type,&in_b);
        set_type(all_type,&in_c);
        set_type(all_type,&out_1);
        set_type(all_type,&out_2);
        //info
        if(all_type!="fp16"&&all_type!="fp32"&&all_type!="fp64")
            printf("FAIL TO set all_type,default all_type is fp32\n");
        else
            printf("SUCCESSFULLY set all_type:: %s\n",all_type.c_str());    
    }
    else{
        std::string in1_type = argc_string(argc,argv,"type1=");
        std::string in2_type = argc_string(argc,argv,"type2=");
        std::string in3_type = argc_string(argc,argv,"type3=");
        std::string o_type = argc_string(argc,argv,"typeo=");
        set_type(in1_type,&in_a);
        set_type(in2_type,&in_b);
        set_type(in3_type,&in_c);
        set_type(o_type,&out_1);
        set_type(o_type,&out_2);
        //info
        if(in1_type!="fp16"&&in1_type!="fp32"&&in1_type!="fp64")
            printf("FAIL TO set in1_type,default type is fp32\n");
        else
            printf("SUCCESSFULLY set in1_type:: %s\n",in1_type.c_str());
        if(in2_type!="fp16"&&in2_type!="fp32"&&in2_type!="fp64")
            printf("FAIL TO set in2_type,default type is fp32\n");
        else
            printf("SUCCESSFULLY set in2_type:: %s\n",in2_type.c_str());
        if(in3_type!="fp16"&&in3_type!="fp32"&&in3_type!="fp64")
            printf("FAIL TO set in3_type,default type is fp32\n");
        else
            printf("SUCCESSFULLY set in3_type:: %s\n",in3_type.c_str());
        if(o_type!="fp16"&&o_type!="fp32"&&o_type!="fp64")
            printf("FAIL TO set o_type,default type is fp32\n");
        else
            printf("SUCCESSFULLY set o_type:: %s\n",o_type.c_str());
    }
    
////////////////
//set ref model
////////////////
    int rnd_mode=0;
    std::string rnd_mode_str="rnd_mode=";

//set rnd_mode
    rnd_mode=argc_int(argc,argv,rnd_mode_str);
    if(rnd_mode ==0)
        printf("SUCCESSFULLY set rnd_mode::TO_ZERO\n");
    if(rnd_mode ==1)
        printf("SUCCESSFULLY set rnd_mode::TO_DOWNWARD\n");
    if(rnd_mode ==2)
        printf("SUCCESSFULLY set rnd_mode::TO_UPWARD\n");
    if(rnd_mode ==3)
        printf("SUCCESSFULLY set rnd_mode::TO_NEAREST\n");

//set ref model
    ref my_ref1;
    std::string ref_string;
    int         ref_flag=0;

    auto ref_func = my_ref1.functionTable.end();
    for(int i=0;i<argc;i++){
        ref_string = argv[i];
        ref_func   = my_ref1.functionTable.find(ref_string);
        if(ref_func != my_ref1.functionTable.end())
        {   
            ref_flag=1;
            break;
        }
        else{
            ref_func = my_ref1.functionTable.find("mul");
        }
    }

//info
    if(!ref_flag)
        printf("FAIL TO set ref_model,default ref_model is mul\n");
    else
        printf("SUCCESSFULLY set ref_model:: %s\n",ref_string.c_str());

////////////////
//set cmodel
////////////////
    cmodel my_cmodel1;
    
    //set debug mode
    auto c_info=my_cmodel1.InfoTable.end();
    std::string info_string;
    for(int i=0;i<argc;i++){
        info_string = argv[i];
        c_info = my_cmodel1.InfoTable.find(info_string);
        if(c_info != my_cmodel1.InfoTable.end())
        {
            break;
        }
        else{
            c_info = my_cmodel1.InfoTable.find("mul");
        }
    }

    if(argc_bool(argc,argv,"c_debug"))
    {   
        c_info->second->debug=1;
    }
    else{
        c_info->second->debug=0;
    }
    std::string cmodel_string;
    
    //set cmodel and c variables
    auto c_func = my_cmodel1.functionTable.end();
    auto vari   = my_cmodel1.VariablesTable.end();

    int  c_flag = 0;
    for(int i=0;i<argc;i++){
        cmodel_string = argv[i];
        c_func = my_cmodel1.functionTable.find(cmodel_string);
        vari   = my_cmodel1.VariablesTable.find(cmodel_string);

        if((c_func != my_cmodel1.functionTable.end()) && (vari != my_cmodel1.VariablesTable.end()))
        {
            c_flag=1;
            break;
        }
        else{
            c_func = my_cmodel1.functionTable.find("mul");
            vari   = my_cmodel1.VariablesTable.find("mul");
        }
    }
    //info
    if(!c_flag)
        printf("FAIL TO set cmodel,default cmodel is mul\n");
    else
        printf("SUCCESSFULLY set cmodel:: %s\n",cmodel_string.c_str());

////////////////
//set rtl_model
////////////////
    std::string simv_executable=SIMV_EXECUTABLE_PATH;
    // std::string simv_executable="/home/liuyunqi/ymyu/libhwmath/build_sv/./simv";
    std::string quiet   = " -q";
    std::string debug   = " +RTL_DEBUG";
    std::string command;
    if(argc_bool(argc,argv,"DEBUG_RTL"))
        command = simv_executable + quiet +debug;
    else
        command = simv_executable + quiet;
    
    std::string rtl_model;
    int         rtl_flag=0;
    for (int i = 0; i < argc; ++i) {
        std::string model_argv = argv[i];
        auto iter = rtl_map.find(model_argv);
        if (iter != rtl_map.end()) {
            rtl_model = iter->second;  // Assign the value to rtl_model
            rtl_flag  = 1;
            break;
        } else {
            rtl_model = rtl_map["mul"];  // Assign the default value "mul"
        }
    }

    command = command + rtl_model;
    std::cout << command << std::endl;

    Pipe pipe1(command);
    auto rtl_func=&Pipe::verif_inout;
    
    //info
    if(!rtl_flag)
        printf("FAIL TO set rtl_model,default rtl_model is mul\n");
    else
        printf("SUCCESSFULLY set rtl_model:: %s\n",rtl_model.c_str());

////////////////
//set checker model
////////////////
    Checker checker1;
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
        }
    }
    //info
    if(checker_string!="assert")
        printf("FAIL TO set checker,default checker is mul\n");
    else
        printf("SUCCESSFULLY set checker:: %s\n",checker_string.c_str());

//////////////////////////
// choose two comparision
//////////////////////////
using mdltype= std::function<std::array<int,5>(const FpBase& , const FpBase& , const FpBase&, const int&, FpBase *)>;
mdltype checker_1_func;
mdltype checker_2_func;

//use a table to handle the inputs
std::unordered_map<std::string,mdltype> modeltable;
modeltable["ref"]=(ref_func->second);
modeltable["cmodel"]=(c_func->second);
modeltable["rtl"]=std::bind(rtl_func,&pipe1,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);


std::string checker_1_str=argc_string(argc,argv,"checker1=");
std::string checker_2_str=argc_string(argc,argv,"checker2=");


auto checker_1 = modeltable.find(checker_1_str);
auto checker_2 = modeltable.find(checker_2_str);

int checker_1_flag = 0;
int checker_2_flag = 0;

if(checker_1 != modeltable.end())
{
    checker_1_func = checker_1->second;
    checker_1_flag = 1;
}
else{
    checker_1_func = modeltable["ref"];
    // printf("into default check1:ref");
}

if(checker_2 != modeltable.end())
    {
        checker_2_func = checker_2->second;
        checker_2_flag = 1;

    }
    else{
        checker_1_func = modeltable["cmodel"];
        // printf("into default check2:cmodel");
    }

//info
if(checker_1 == modeltable.end())
    printf("FAIL TO set checker_model_1,default checker_model_1 is ref\n");
else
    printf("SUCCESSFULLY set checker_model_1:: %s\n",checker_1_str.c_str());

if(checker_2 == modeltable.end())
    printf("FAIL TO set checker_model_2,default checker_model_2 is cmodel\n");
else
    printf("SUCCESSFULLY set checker_model_2:: %s\n",checker_2_str.c_str());


//////////////////////////////
// enable rtl and cmodel_debug
//////////////////////////////

bool rtl_c_debug;
rtl_c_debug=((checker_1_str=="cmodel")&&(checker_2_str=="rtl"))||((checker_1_str=="rtl")&&(checker_2_str=="cmodel"));

////////////////
// start test
////////////////
    bool fail=0;
    std::array<int,5>status1;
    std::array<int,5>status2;
    std::array<int,5>status1_old={0,0,0,0,0};
    std::array<int,5>status2_old={0,0,0,0,0};

    for(int i=0;i<test_times;i++){
        if(argc_bool(argc,argv,"c_debug")){
            in_a.sign=1;
            in_a.expo=155;
            in_a.mant=3801088;
            in_b.sign=0;
            in_b.expo=94;
            in_b.mant=8357760;
            in_c.sign=0;
            in_c.expo=0;
            in_c.mant=0;
        }
        else{
            in1_func->second(&in_a);
            in2_func->second(&in_b);
            in3_func->second(&in_c);
        }
        status1={0,0,0,0,0};
        status2={0,0,0,0,0};
        status1=checker_1_func(in_a,in_b,in_c,rnd_mode,&out_1);
        status2=checker_2_func(in_a,in_b,in_c,rnd_mode,&out_2);
        fail   =checker_func->second(out_1,out_2,status1,status2);
        if(fail){
            if(rtl_c_debug){
                checker1.compareVariables(*vari->second,pipe1.VariablesTable);
            }
            printf("in_a:\n");
            in_a.print();
            printf("in_b:\n");
            in_b.print();
            // printf("in_c:\n");
            // in_c.print();
            printf("output one is ---------------\n");
            out_1.print();
            printf("----------------------------\n");
            printf("output two is ---------------\n");  
            out_2.print();
            printf("----------------------------\n");
            break;
        }
        status1_old=status1;
        status2_old=status2;
    }
    return fail;
}

bool Tester::argc_bool(int argc, char **argv, const std::string&prefix){
    bool value = false;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0){
            value=true;
        }
    }
    return value;
}

std::string Tester::argc_string(int argc,char **argv,const std::string&prefix){
    std::string res="fail";
    for (int i =0 ; i<argc ; i++){
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0)
            res = arg.substr(prefix.size());
    }
    return res;
}

int Tester::argc_int(int argc,char **argv,const std::string&prefix){
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

void Tester::set_type(std::string type_str,FpBase* a){
    if(type_str=="fp16"){
        (*a).expo_w = 5;
        (*a).mant_w = 10;
    }
    else if(type_str=="fp32"){
        (*a).expo_w = 8;
        (*a).mant_w = 23;
    }
    else if(type_str=="fp64"){
        (*a).expo_w = 11;
        (*a).mant_w = 52;
    }
    else {
        (*a).expo_w = 8;
        (*a).mant_w = 23;
    }
}

#endif