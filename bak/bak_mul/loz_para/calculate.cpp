#include "calculate.h"
void Calculate::generate_s(uint32_t* current_s, uint64_t* input_num,int loop) {
    uint32_t input_s=0;
    uint64_t input_last;
    for(int i=0;i<loop;i++){
        input_last = *input_num & 0x01;
        input_s=*(uint32_t*)&input_last;
        *current_s|= input_s; 
        *input_num>>=1;
    }
}
bool Calculate::is_zero(uint32_t num){
    uint32_t num_31=num&0x7fffffff;
    return !num_31;
}

bool Calculate::is_inf(uint32_t num){
    return (num&0x7fffffff)==FP32_INF;
}

bool Calculate::is_nan(uint32_t num){
    return (num&0x7fffffff)>FP32_INF;
}

bool Calculate::is_special(uint32_t num){ 
    // printf("num&0x7fffffff is %b\n",num&0x7fffffff);
    // printf("FP32_REG_MIN is   %b\n",FP32_REG_MIN);
    return ((num&0x7fffffff)<FP32_REG_MIN);

}

uint64_t Calculate::get_last_n_bit(uint64_t num,int n){
    uint64_t n_mast=(static_cast<uint64_t>(1) << n)-1;
    return num&n_mast;
}

uint64_t Calculate::get_the_n_bit(uint64_t num,int n){
    uint64_t n_mast=static_cast<uint64_t>(1) << (n-1);
    return num&n_mast;
}

