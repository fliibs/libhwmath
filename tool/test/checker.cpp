#include "checker.h"
bool Checker::assert_T(const FpBase& input1, const FpBase& input2,const std::array<int,5> arr1,const std::array<int,5> arr2){
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
    else{
        std::cerr << "op failed"<<std::endl;
        fail=1;
    }
    return fail;
}

void Checker::printVariables(std::unordered_map <std::string, mp::cpp_int> table) {
    for (const auto& pair : table) {
        std::cout << "Variable Name: " << pair.first << ", Value: " << pair.second << std::endl;
    }
}

void Checker::compareVariables(std::unordered_map <std::string, mp::cpp_int> table1,std::unordered_map <std::string, mp::cpp_int> table2){
    for (const auto&pair:table1){
        const auto&name=pair.first;
        const auto&value1=pair.second;
        if(table2.count(name)>0){
            const auto&value2=table2.at(name);
            if(value1!=value2){
                printVariables(table1);
                printVariables(table2);
                std::cout<<"variable name:"<<name<<" differs. Value in table1 is: "<<value1<<". Value in table2 is: "<<value2<<std::endl;
                break;
            }
        }
    }
}