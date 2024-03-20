#include "pipe.h"
std::array<int,5> Pipe::verif_inout(FpBase a,FpBase b,FpBase c,int rnd_mode,FpBase* result){
    std::array<int,5> arr;
    VariablesTable.clear();
    int               status;
    uint64_t          res;
    uint64_t          value_a;
    uint64_t          value_b;
    uint64_t          value_c;

    value_a=((a.sign<<(a.expo_w+a.mant_w))+(a.expo<<a.mant_w)+a.mant);
    value_b=((b.sign<<(b.expo_w+b.mant_w))+(b.expo<<b.mant_w)+b.mant);
    value_c=((c.sign<<(c.expo_w+c.mant_w))+(c.expo<<c.mant_w)+c.mant);

    to_sv<<value_a<<" "<<value_b<<" "<<value_c<<" "<<rnd_mode<<std::endl;
    
    std::string      name;
    std::string      value;
    size_t           equalPos;
    size_t           commaPos;
    //
    //has to change according to fp64
    //
    uint64_t         value_int; 
    mp::cpp_int      value_lint;

    while(model_rtl.running()&&std::getline(from_sv,line)){
        std::cout<<line<<std::endl;
        if(line.compare(0,7,"status=")==0){
            std::string status_str=line.substr(7);
            std::istringstream(status_str) >> status;
            break;
        }
        if(line.compare(0,4,"res=")==0){
            std::string res_str=line.substr(4);
            std::istringstream(res_str) >> res;
        }
        //
        //write from RTL to VariableTable
        //
        equalPos = line.find("=");
        commaPos=line.find(",");

        if (equalPos!=std::string::npos)
        {
            name=line.substr(commaPos+1,equalPos-commaPos-1);
            value=line.substr(equalPos+1);
            //
            //has to change according to fp64
            //
            std::istringstream(value) >> value_int; 
            value_lint = value_int;
            addVariable(name,value_int);
        }

    }

    // printf("res is %lb\n",res);
    // printf("res is %lu\n",res);
    (*result).sign  = res >> ((*result).expo_w + (*result).mant_w);
    uint64_t expo_mask;
    uint64_t mant_mask;
    expo_mask = get_mask((*result).expo_w);
    mant_mask = get_mask((*result).mant_w);
    // printf("expo_mask is %d\n",expo_mask);
    // printf("mant_mask is %lb\n",mant_mask);
    (*result).expo  = (res >> (*result).mant_w) & expo_mask;
    // printf("res shift is %lb\n",res >> (*result).mant_w);
    (*result).mant  = res&mant_mask; 
    // printf("res     is %lb\n",res);
    // printf("mant_mask is %lb\n",mant_mask);
    // printf("(*result).mant  is %lb\n",(*result).mant );
    for(int i=0 ; i<5 ; i++){
        arr[i]=status&0x01;
        status=status>>1;
    }
    return arr;
}

void Pipe::addVariable(const std::string &variableName,mp::cpp_int value){
    VariablesTable[variableName]=value;
}

void Pipe::removeVariable(const std::string &variableName){
    VariablesTable.erase(variableName);
}
