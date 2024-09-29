#include "pipe.h"
std::array<int,5> Pipe::verif_inout(FpBase a,FpBase b,FpBase c,int rnd_mode,FpBase* result){
    std::array<int,5> arr;
    VariablesTable.clear();
    int               status;
    uint64_t          res;
    uint64_t          value_a;
    uint64_t          value_b;
    uint64_t          value_c;

    uint64_t value_a_sign;
    uint64_t value_a_expo;
    uint64_t value_b_sign;
    uint64_t value_b_expo;
    uint64_t value_c_sign;
    uint64_t value_c_expo;
    value_a_sign = static_cast<uint64_t>(a.sign)<<(a.expo_w+a.mant_w);
    value_a_expo = static_cast<uint64_t>(a.expo)<<(a.mant_w)         ;
    value_a      = value_a_sign +  value_a_expo +  a.mant            ;

    value_b_sign = static_cast<uint64_t>(b.sign)<<(b.expo_w+b.mant_w);
    value_b_expo = static_cast<uint64_t>(b.expo)<<(b.mant_w)         ;
    value_b      = value_b_sign +  value_b_expo +  b.mant            ;
    
    value_c_sign = static_cast<uint64_t>(c.sign)<<(c.expo_w+c.mant_w);
    value_c_expo = static_cast<uint64_t>(c.expo)<<(c.mant_w)         ;
    value_c      = value_c_sign +  value_c_expo +  c.mant            ;
    // value_a=((a.sign<<(a.expo_w+a.mant_w))+(a.expo<<a.mant_w)+a.mant);
    // value_b=((b.sign<<(b.expo_w+b.mant_w))+(b.expo<<b.mant_w)+b.mant);
    // value_c=((c.sign<<(c.expo_w+c.mant_w))+(c.expo<<c.mant_w)+c.mant);

    info1.print_cpp_int(value_a,"b");
    info1.print_cpp_int(a.sign,"b");
    info1.print_cpp_int(a.expo,"b");
    info1.print_cpp_int(a.mant,"b");

    info1.debug_printf("value_b:\n");
    info1.print_cpp_int(value_b,"b");
    info1.print_cpp_int(b.sign,"b");
    info1.print_cpp_int(b.expo,"b");
    info1.print_cpp_int(b.mant,"b");

    // info1.print_cpp_int(value_c,"b");
    // info1.print_cpp_int(c.sign,"b");
    // info1.print_cpp_int(c.expo,"b");
    // info1.print_cpp_int(c.mant,"b");

    std::cout<<value_a<<" "<<value_b<<" "<<value_c<<" "<<rnd_mode<<std::endl;
    std::cout<<a.sign<<" "<<a.expo<<" "<<a.mant<<" "<<rnd_mode<<std::endl;
    std::cout<<b.sign<<" "<<b.expo<<" "<<b.mant<<" "<<rnd_mode<<std::endl;

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
        commaPos = line.find(",");

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

    (*result).sign  = res >> ((*result).expo_w + (*result).mant_w);
    uint64_t expo_mask;
    uint64_t mant_mask;
    expo_mask = get_mask((*result).expo_w);
    mant_mask = get_mask((*result).mant_w);
    (*result).expo  = (res >> (*result).mant_w) & expo_mask;
    (*result).mant  = res&mant_mask; 
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
