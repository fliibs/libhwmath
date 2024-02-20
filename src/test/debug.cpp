#include "debug.h"
void Debug::debug_printf(const char* cmd, ...){
if(debug){  
    // printf("debug is %d\n",debug);  
    va_list args; // 用来存储单个参数
    va_start(args, cmd); // 用args指向可变参数的第一个参数
    while (*cmd != '\0') {
        if (*cmd == '%' && *(cmd + 1) == 's') {
            const char* str_arg = va_arg(args, const char*);
            std::cout << ' ' << str_arg;
            cmd += 2; // jump '%s'
        } else if (*cmd == '%' && *(cmd + 1) == 'd') {
            int int_arg = va_arg(args, int);
            std::cout << ' ' << int_arg;
            cmd += 2; // jump '%d'
        } else if (*cmd == '%' && *(cmd + 1) == 'u') {
            uint32_t uint32_t_arg = va_arg(args, uint32_t);
            std::cout << ' ' << static_cast<int>(uint32_t_arg);
            cmd += 2; // jump '%u'
        } else if (*cmd == '%' && *(cmd + 1) == 'l'&& *(cmd + 2) == 'u') {
            uint64_t uint64_t_arg = va_arg(args, uint64_t);
            // std::cout << ' ' << static_cast<int>(uint64_t_arg);
            std::cout << ' ' << uint64_t_arg;
            cmd += 3; // jump '%lu'
        } else if (*cmd == '%' && *(cmd + 1) == 'f') {
            float float_arg = va_arg(args, float); // use double for %f
            std::cout << ' ' << float_arg;
            cmd += 2; // jump '%f'
        } else if (*cmd == '%' && *(cmd + 1) == 'l'&& *(cmd + 2) == 'f') {
            double double_arg = va_arg(args, double);
            std::cout << ' ' << double_arg;
            cmd += 3; // jump '%lf'
        } else if (*cmd == '%' && *(cmd + 1) == 'b') {
            uint32_t uint32_arg = va_arg(args, uint32_t);
            std::bitset<32> binary_representation(uint32_arg);
            std::cout << ' ' << binary_representation;
            cmd += 2; // jump '%b'
        } else if (*cmd == '%' && *(cmd + 1) == 'l' && *(cmd + 2) == 'b') {
            uint64_t uint64_arg = va_arg(args, uint64_t);
            std::bitset<64> binary_representation(uint64_arg);
            std::cout << ' ' << binary_representation;
            cmd += 3; // jump '%lb'
        // } else if(*cmd == '%' && *(cmd + 1) == 'm' && *(cmd + 2)=='b'){
        //     mp::cpp_int cpp_int_arg = va_arg(args, mp::cpp_int);
        //     std::cout << cpp_int_arg;
        //     cmd +=3;
        } else {
            std::cout << *cmd;
            cmd++;
        }
    }
    va_end(args); 
    std::cout << std::endl;
}
else{
    // printf("debug is %d\n",debug);  
}
}

void Debug::print_cpp_int(const mp::cpp_int& num,const std::string& str) {
if(debug){
    // std::cout<<"------------------------------"<<std::endl;

    // std::cout<<"num in cpp_int format:"<<num<<std::endl;
    // uint64_t ref_h_1=mantissa_75_bit_h;
    // uint64_t ref_l_1=0;
    // uint64_t ref_h_1=mant_get_75_bit_h;
    // uint64_t ref_l_1=mant_get_50_bit_l;
    uint64_t ref_h_1=0;
    uint64_t ref_l_1=mantissa_get_50_bit;

    uint64_t ref_h_2=mant_get_75_bit_h;
    uint64_t ref_l_2=mant_get_50_bit_l;
    std::bitset<64> ref_h_bin_1(ref_h_1);
    std::bitset<64> ref_l_bin_1(ref_l_1);
    std::bitset<64> ref_h_bin_2(ref_h_2);
    std::bitset<64> ref_l_bin_2(ref_l_2);

    uint64_t ref_50=mantissa_get_50_bit;
    std::bitset<64> ref_50_bin(ref_50);

    int cut_in_bit=64;
    
    std::string check_string="b";
    mp::cpp_int a_in_h;
    mp::cpp_int a_in_l;
    // mp::cpp_int a_low;
    // if(num<ref_l_1)

    a_in_h=num>>cut_in_bit;
    a_in_l=num&mantissa_get_64_bit;
    // std::cout<<"a_in_h:"<<a_in_h<<std::endl;
    // std::cout<<"a_in_l:"<<a_in_l<<std::endl;

    std::string oss_h=mp::to_string(a_in_h);
    std::string oss_l=mp::to_string(a_in_l);
    // std::cout<<"oss_h:"<<oss_h<<std::endl;
    // std::cout<<"oss_l:"<<oss_l<<std::endl;
    uint64_t output_h=std::stoul(oss_h,nullptr,10);
    uint64_t output_l=std::stoul(oss_l,nullptr,10);

    std::bitset<64> output_h_bin(output_h);
    std::bitset<64> output_l_bin(output_l);
    // std::cout<<"output_h:"<<output_h_bin<<std::endl;
    // std::cout<<"output_l:"<<output_l_bin<<std::endl;
    // std::cout<<"out_50_r:"<<ref_50_bin<<std::endl;

    if(str==check_string){
        std::cout<<output_h_bin;
        std::cout<<output_l_bin<<std::endl;
        std::cout<<"  "<<"ref_75:"<<std::endl;
        std::cout<<ref_h_bin_1;
        std::cout<<ref_l_bin_1<<std::endl;
        // std::cout<<"ref_48:"<<std::endl;
        // std::cout<<ref_h_bin_2;
        // std::cout<<ref_l_bin_2<<std::endl;
   }
   else{
        std::cout<<output_h;
        std::cout<<output_l<<std::endl;
   }
    // std::cout<<"------------------------------"<<std::endl;

}
else{

    }
}

void Debug::print_array(int* array, int array_length){
if(debug){
    for(int i=0;i<array_length;i++)
    {
        // std::cout<<i<<":"<<std::endl;
        std::cout<<array[array_length-1-i];
    //    std::cout<<"  "<<array[i]<<std::endl;
    }
    std::cout<<std::endl;
}
else{
    
}
}