#include "cal.h"
#include "../test/info.h"

//exponent
uint32_t set_expo_max(int expo_w){
    uint32_t result=0;
    for(int i=0;i<expo_w;i++){
        result = 1+(result<<1);
    }
    return result;
}

uint32_t get_expo_sign_bit(int expo_w){
    uint32_t result=0;
    result = 1<<(expo_w+1);
    return result;
}

bool expo_whe_max(uint32_t expo,int expo_w){
    bool result;
    int expo_max=set_expo_max(expo_w);
    result = static_cast<bool>(expo_max==expo);
    return result;
}

//mant
mp::cpp_int get_mant_av_bits(int av_nums){
    mp::cpp_int result=0;
    for(int i=0;i<av_nums;i++){
        result = 1+(result<<1);
    }
    return result; 
}

mp::cpp_int get_mant_av_top_bit(int av_nums){
    mp::cpp_int result=0;
    result = static_cast<mp::cpp_int>(1) << (av_nums-1);
    return result;
}

mp::cpp_int get_nan(int in_num,int out_num, mp::cpp_int in_mant){
    mp::cpp_int result=0;
    int         dif; 
    mp::cpp_int cmpl_bits;
    mp::cpp_int mant_cmpl;
    mp::cpp_int mant_lack1_bits;
    if(in_num<out_num){
        dif         = out_num - in_num;
        cmpl_bits   = get_mant_av_bits(dif);
        mant_cmpl   = (in_mant << dif) + cmpl_bits;
    }
    else if(in_num>out_num) {
        dif         = in_num - out_num;
        mant_cmpl   = in_mant >> dif;
    }
    else
        mant_cmpl   = in_mant;

    mant_lack1_bits = get_mant_av_top_bit(out_num);
    
    result          = mant_lack1_bits | mant_cmpl;

    return result;
}

// get_rnd_bit(get_rnd_g_r,i<=res_mant_w);

get_rnd_bit(get_rnd_g,i==(mant_av_nums-res_mant_w-1));
 
get_rnd_bit(get_rnd_r,i==(mant_av_nums-res_mant_w));

//cal
int get_zero_nums(mp::cpp_int *mant_in,int mant_av_nums){
    int zero_nums     = 0;
    int zero_nums_len = 0;
    zero_nums_len     = std::ceil(log2(mant_av_nums));

    mp::cpp_int mant_av_top_bit;
    mant_av_top_bit   = get_mant_av_top_bit(mant_av_nums);
    int ary_w       =0; 
    int zero_num_bit=0;
    int pow_expo    =0;
    double ary_w_dl;
    for (int i=0 ; i< zero_nums_len;i++){
        pow_expo        = zero_nums_len-1-i;
        ary_w           = std::ceil(std::pow(2,pow_expo));
        zero_num_bit    = !detect_one(&*mant_in,ary_w,mant_av_top_bit);
        zero_nums       = (zero_nums<<1) + (zero_num_bit);
    }
    return zero_nums;
}

int detect_one(mp::cpp_int *mant_in,int width,mp::cpp_int mant_av_top_bit){
    int one_exist        = 0;
    mp::cpp_int mant_rcd = *mant_in;
    while(width>0){
        one_exist       |= ((*mant_in)&(mant_av_top_bit))?1:0;
        width           -= 1;
        (*mant_in)     <<= 1;
    }
    (*mant_in)           = one_exist ? mant_rcd : (*mant_in);
    return one_exist;
}

uint64_t get_mask(const int&width){
    uint64_t res=0;
    for(int i=0;i<width;i++){
        res = 1+(res<<1);
    }   
    return res;
}


