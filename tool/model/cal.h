#ifndef cal_h
#define cal_h
#include "struct.h"
#include <cmath>

#define get_rnd_bit(function_name,condition) \
    mp::cpp_int function_name(int mant_av_nums,int res_mant_w) { \
        mp::cpp_int result =0; \
        mp::cpp_int bit_set=0; \
        for(int i=0;i<mant_av_nums;i++) { \
            bit_set = (condition) ? 1:0; \
            result  = (result<<1) + bit_set; \
        } \
        return result; \
    }

uint32_t    set_expo_max(int expo_w);
uint32_t    get_expo_sign_bit(int expo_w);
bool        expo_whe_max(uint32_t expo,int expo_w);

mp::cpp_int get_mant_av_bits(int av_nums );
mp::cpp_int get_mant_av_top_bit(int av_nums);
mp::cpp_int get_nan(int in_num,int out_num, mp::cpp_int in_mant);
mp::cpp_int get_rnd_g_r(int mant_av_nums,int res_mant_w);
mp::cpp_int get_rnd_g(int mant_av_nums,int res_mant_w);
mp::cpp_int get_rnd_r(int mant_av_nums,int res_mant_w);

int         get_zero_nums(mp::cpp_int *mant_in,int mant_av_nums);
int         detect_one(mp::cpp_int *mant_in,int width,mp::cpp_int mant_av_top_bit);

#endif //cal_h