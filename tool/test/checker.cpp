#include "checker.h"
#include "../model/cal.h"
bool Checker::assert_T(const FpBase& input1, const FpBase& input2,const std::array<int,5> arr1,const std::array<int,5> arr2){
    bool input1_nan;
    bool input2_nan;
    uint32_t expo_max=set_expo_max(input1.expo_w);
    input1_nan =(input1.expo==expo_max) && (input1.mant!=0);
    input2_nan =(input2.expo==expo_max) && (input2.mant!=0);

    bool op_mul = arg_in.op=="mul";
    bool fp16_trigger_0inf   =(arg_in.corner[0]=="zero" && arg_in.corner[1]=="inf")||(arg_in.corner[1]=="inf" && arg_in.corner[0]=="zero");
    bool fp16_trigger_0reg   =(arg_in.corner[0]=="zero" && arg_in.corner[1]=="reg")||(arg_in.corner[1]=="reg" && arg_in.corner[0]=="zero") && input1_nan && input2_nan;
    bool fp16_trigger_infreg =(arg_in.corner[0]=="inf"  && arg_in.corner[1]=="reg")||(arg_in.corner[1]=="reg" && arg_in.corner[0]=="inf")  && input1_nan && input2_nan;

    bool fp16  = arg_in.type[3]=="fp16";

    bool fail=0;
    if((input1==input2)&&(arr1==arr2)){
        fail=0;
    }
    else if(arr1!=arr2){
        std::cerr << "excepetion assertion failed"<<std::endl;
        status_check("NV",4);
        status_check("DZ",3);
        status_check("OF",2);
        status_check("UF",1);
        status_check("NX",0);
        fail=1;
    }
    else if(fp16 && op_mul && (fp16_trigger_0inf || fp16_trigger_0reg ||fp16_trigger_infreg))
    {
        if(input1.expo==input2.expo && (input1.mant>>(input1.mant_w-1)) ==(input1.mant>>(input1.mant_w-1)))
            fail=0;
    }
    else {
        std::cerr << "op failed"<<std::endl;
        fail=1;
    }

    return fail;
}

void Checker::printVariables(std::map <std::string, mp::cpp_int> table) {
    for (const auto& pair : table) {
        std::cout << "Variable Name: " <<std::setw(20)<< pair.first << ", Value: " <<std::setw(10)<< pair.second << std::endl;
    }
}

void Checker::compareVariables(std::map <std::string, mp::cpp_int> table1,std::map <std::string, mp::cpp_int> table2){
    int flag = 0;
    for (const auto&pair:table1){
        const auto&name=pair.first;
        const auto&value1=pair.second;
        if(table2.count(name)>0){
            const auto&value2=table2.at(name);
            if(value1!=value2){
                flag =1;
                std::cout<<"variable name:"<<std::setw(10)<<name<<std::endl;
                std::cout<<std::setw(10)<<" Value in table1 is: "<<std::setw(10)<<value1<<std::setw(10)<<". Value in table2 is: "<<std::setw(10)<<value2<<std::endl;
                // break;
            }
        }
    }
    if(flag)
        printf("---------\n");
        printf("table2:\n");
        printf("---------\n");

        printVariables(table2);
        printf("---------\n");
        printf("table1:\n");
        printf("---------\n");

        printVariables(table1);

}