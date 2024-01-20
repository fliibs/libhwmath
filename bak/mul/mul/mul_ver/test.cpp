#include "test.h"
int get_length(uint64_t num){
    int length=0;
    while(num){
        num>>=1;
        ++length;
    }
    return length;
}

uint32_t get_last_22_bit(uint64_t num){
    uint32_t last_22_bit=num&0x3fffff;
    return last_22_bit;
}

void debug_printf(const char* cmd, ...){
#if DEBUG_ENABLE
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
            float float_arg = va_arg(args, double); // use double for %f
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
        // } else if (*cmd == '%' && *(cmd + 1) == 'l' && *(cmd + 2) == 'b') {
        //     uint64_t uint64_arg = va_arg(args, uint64_t);
        //     printf(" %lb",uint64_arg);
        //     cmd += 3;
        } else {
            std::cout << *cmd;
            cmd++;
        }
    }
    va_end(args); 
    std::cout << std::endl;
#endif
}