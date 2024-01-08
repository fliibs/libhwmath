#include "test.h"
#include "fp32.h"
#include "calculate.h"

Fp32::Fp32() {
        sign = 0;
        exponent = 0;
        mantissa = 0;
    }
    // constructor, convert a float to Fp32 type
Fp32::Fp32(float x) {
        uint32_t bits = *(uint32_t*)&x; // get the binary representation of the float
        sign     = bits >> 31;          // get the sign bit
        exponent = (bits >> 23) & 0xff; // get the exponent bit
        mantissa = bits & 0x7fffff;     // get the mantissa bit
    }
    // conversion function, convert a Fp32 type to float
Fp32::operator float() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        float x = *(float*)&bits;   // get the float value
        return x;
    }

Fp32::operator uint32_t() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        return bits;
    }

uint32_t Fp32::to_uint32() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        return bits;
    }

Fp32 Fp32::operator* (const Fp32 &rhs) {
        return mul(rhs);
    }

void Fp32::print() {
    printf("value      : %12f\n", (float)*this);
    printf(" - sign    : %12ld\n",sign);
    printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
    printf(" - mantissa: %12ld,%64lb, %f\n",mantissa,mantissa+ (1 << 23),(float)(mantissa + (1 << 23))/(1<<23));
    printf("\n");
    }

Fp32 Fp32::mul(const Fp32 &rhs){
    Fp32 result(0.0f);
    //--------------to determine 0, NAN and INF
    uint64_t a_expo_is_zero  =   (exponent==0)                        ?1:0;                      //|exponent
    uint64_t b_expo_is_zero  =   (rhs.exponent==0)                    ?1:0;                      //|rhs.exponent
    uint64_t a_expo_is_ff    =   (exponent==255)                      ?1:0;                      //&exponent
    uint64_t b_expo_is_ff    =   (rhs.exponent==255)                  ?1:0;                      //&rhs.exponent
    uint64_t a_mant_is_00    =   (mantissa==0)                        ?1:0;           
    uint64_t b_mant_is_00    =   (rhs.mantissa==0)                    ?1:0;
    
    uint64_t a_is_0          =   (a_expo_is_zero&&a_mant_is_00)       ?1:0;                         
    uint64_t b_is_0          =   (b_expo_is_zero&&b_mant_is_00)       ?1:0;
    uint64_t a_is_nan        =   (a_expo_is_ff  &&(!a_mant_is_00))    ?1:0;
    uint64_t b_is_nan        =   (b_expo_is_ff  &&(!b_mant_is_00))    ?1:0;
    uint64_t a_is_inf        =   (a_expo_is_ff  &&a_mant_is_00)       ?1:0;
    uint64_t b_is_inf        =   (b_expo_is_ff  &&b_mant_is_00)       ?1:0;

    //--------------debuging 
    debug_printf("a_is_0 is %lu",a_is_0);
    debug_printf("b_is_0 is %lu",b_is_0);
    debug_printf("a_is_inf is %lu,a_is_inf");
    debug_printf("b_is_inf is %lu,b_is_inf");
    debug_printf("r_is_nan is %lu,r_is_nan");

    //--------------generate sign
    result.sign              =   sign^rhs.sign;

    //--------------generate expontemp_exponentent,input exponent is 8 bit,temperory exponent is 10 bit, 1 bit for expantion,1 bit for sign
    uint32_t temp_expo;
    temp_expo                =   exponent+rhs.exponent-0x7f+a_expo_is_zero+b_expo_is_zero;       //can be optimized into -0x80+1

    //--------------generate mantissa,and generating the final exponent results
    uint64_t temp_mant;
    uint64_t temp_mant_last_47;
    uint64_t a_mant_total;
    uint64_t b_mant_total;
    uint32_t zero_nums;

    uint64_t mant_long;
    uint32_t man_expo;

    uint32_t bit_s_record;
    uint32_t shift_left;
    uint32_t shift_right;
    //loz and mul in para
    a_mant_total             =   ((1-a_expo_is_zero)<<23)+mantissa;
    b_mant_total             =   ((1-b_expo_is_zero)<<23)+rhs.mantissa;
    temp_mant                =   a_mant_total*b_mant_total;
    temp_mant_last_47        =   temp_mant&mantissa_get_47_bit;
    zero_nums                =   get_the_zero_nums(mantissa, rhs.mantissa,a_expo_is_zero,b_expo_is_zero,temp_mant_last_47);//input 48-bit, top is 0
    bit_s_record             =   0;

    uint64_t temp_mant_l_48=temp_mant&mantissa_48_bit;
    uint64_t temp_mant_ref=0xffffffffffff;
    debug_printf("temp_mant is    %lb",temp_mant);
    debug_printf("temp_mant_48 is %lb",temp_mant_l_48);
    debug_printf("temp_mant_rf is %lb",temp_mant_ref);
    debug_printf("temp_mant_47 is %lb",temp_mant_last_47);
    debug_printf("temp_mant is    %lu",temp_mant);
    debug_printf("temp_mant_47 is %lu",temp_mant_last_47);
    debug_printf("zero_nums is %d",zero_nums);
    

    if((temp_mant&mantissa_48_bit)&&(!(temp_expo&exponent_sign))){                                                             //1x.xxxxxxx
        debug_printf("into shift right");
        man_expo             =   temp_expo+1;
        bit_s_record         =   temp_mant&0x01;
        mant_long            =   temp_mant>>1;
    }
    else{
        debug_printf("----------not into shift right");
        uint32_t expo_sign=temp_expo&exponent_sign;
        debug_printf("exponent sign is %u",expo_sign);
        debug_printf("zero_num is %u",zero_nums);
        uint64_t temp_mant_l_47=temp_mant_last_47&mantissa_47_bit;
        // debug_printf("temp_mant_l_47's 47 bit is %lb",temp_mant_l_47);
        // debug_printf("temp_mant_l_47's 47 bit is %lu",temp_mant_l_47);
        debug_printf("temp_expo is %b",temp_expo);


        if(!(temp_expo&exponent_sign) && ((temp_expo&exponent_num)>zero_nums)){         //0.xxxxxx and normailized
            debug_printf("-----------into 0.xxxx and normalized");
            debug_printf("zero_num is %u",zero_nums);
            debug_printf("temp_expo is %u",temp_expo);
            zero_nums = zero_nums ? zero_nums-1: zero_nums;
            man_expo         =   temp_expo-zero_nums;
            mant_long        =   temp_mant<<zero_nums;
        }
        else{                                                                                                                  //0.xxxxxxx and denormailized
            debug_printf("---------into 0.xxxx and denormalized");
            man_expo         =   0;
            if(!(temp_expo&exponent_sign) && (temp_expo&exponent_num)){                                                     //e-127->-126
                debug_printf("temp_expo>-126,but level shift left");
                debug_printf("temp_expo is %u",temp_expo);
                debug_printf("temp_expo-1 is %u",temp_expo-1);
                shift_left  =   ((temp_expo-1)>63)?63:temp_expo-1;
                mant_long   =   temp_mant<<shift_left;
            }
            else {
                    debug_printf("temp_expo<-126,temp_mant 48 is not 1");
                    debug_printf("temp_expo is %u",temp_expo);
                    debug_printf("1-temp_expo is %u",1-temp_expo);
                    shift_right  =   ((1-temp_expo)>63)?63:1-temp_expo;
                    mant_long   =   temp_mant>>shift_right;
            }
        }                                                                                           
    }
    debug_printf("man_expo is %b",mant_long);
    debug_printf("mant_long is %lb",mant_long);
    debug_printf("---------------------------");
    debug_printf("------into rounding--------");
    debug_printf("---------------------------");
    //--------------rounding
    uint64_t mant_unround          ;
    uint32_t mant_round            ;
    uint32_t round_expo            ;   
    uint32_t mant_short            ;

    uint32_t g                     ;                                                        //24 bit
    uint32_t r                     ;                                                        //23 bit
    uint32_t s                     ;                                                        //22-1 bit
    uint32_t mant_carry            ;
    
    mant_unround    =   mant_long & mantissa_with_g_r;                                      //47-23 bit
    debug_printf("mant_unround is %lb",mant_unround);
    uint64_t mant_unround_s = mant_long & last_22_binary;
    debug_printf("mant_round_s is %lb",mant_unround_s);
    debug_printf("mant_round_r is %lb",last_22_binary);

    s               =   bit_s_record | (mant_long & last_22_binary);
    debug_printf("s is %u",s);
    g               =   mant_unround & g_bit;

    debug_printf("g is %b",g);
    r               =   mant_unround & r_bit;
    debug_printf("r is %lb",r);
    debug_printf("round_mode is %u",round_mode);
    mant_carry      =   (round_mode==3) ? ((r       && (g||s))  ?1:0) : 
                        (round_mode==2) ? ((!result.sign)   ?1:0) :
                        (round_mode==1) ? (result.sign      ?1:0) :
                        0;
    debug_printf("mant_carry is %u",mant_carry);
    mant_round      =   (mant_unround>>23) + mant_carry;
    round_expo      =   (mant_round&the_25_binary)||((!man_expo)&&(mant_round&the_24_binary)) ? (man_expo + 1) : man_expo;

    mant_short      =   (mant_round&the_25_binary) ? ((mant_round>>1) & last_23_binary) : (mant_round & last_23_binary);
    
    //------------generating output,it seems like inf and nan can't be optimized into the current process.
    debug_printf("---------------------------");
    debug_printf("------into masking---------");
    debug_printf("---------------------------");

    uint32_t overflow           ;
    uint32_t result_is_nan      ;
    uint32_t is_inf_nan         ;
    uint32_t mant_unselect      ;                                                          //output mantissa nan is the same as the first input nan
    uint32_t mant_nan           ;
    uint32_t r_is_0nan          ;
    bool     sign_nan           ;
    bool     sign_unselect      ;

    overflow        =   (round_expo>=255)||(man_expo>=255);                                //0xff
    result_is_nan   =   a_is_nan | b_is_nan | (a_is_0&b_is_inf)| (b_is_0&a_is_inf);
    is_inf_nan      =   a_is_nan | b_is_nan | a_is_inf | b_is_inf;
    // mant_unselect   =   ((mantissa & 0x07fffff) > (rhs.mantissa & 0x07fffff)) ? ((1<<22)+(mantissa & last_22_binary)) : ((1<<22)+(rhs.mantissa & last_22_binary));
    mant_unselect   =   ((exponent & 0xff) > (rhs.exponent & 0xff)) ? ((1<<22)+(mantissa & last_22_binary)) : ((1<<22)+(rhs.mantissa & last_22_binary));  
    mant_nan        =   result_is_nan ? mant_unselect : 0;

    r_is_0nan       =   (a_is_0&b_is_inf)| (b_is_0&a_is_inf);
    sign_unselect   =   ((exponent & 0xff) > (rhs.exponent & 0xff)) ? sign : rhs.sign;
    sign_nan        =   result_is_nan ?  (r_is_0nan | sign_unselect): result.sign;

    debug_printf("result_is_nan is %lu",result_is_nan);
    debug_printf("mant_unselect is %lb",mant_unselect);
    debug_printf("mant_nan is %lb",mant_nan);
    uint32_t mant_nan_len = get_length(mant_nan);
    debug_printf("mant_nan_len is %u",mant_nan_len);
    debug_printf("result is %b",result_is_nan);
    if(is_inf_nan){
        debug_printf("into is_inf_nan");
        result.sign         =   sign_nan ;
        result.exponent     =   255;
        result.mantissa     =   mant_nan;
        debug_printf("result.exponent is %lb",result.exponent);
        debug_printf("result.mantissa is %lb",result.mantissa);

    }
    else{
        debug_printf("into regular");
        if(overflow){
            int mode_1        =    round_mode&0x02;
            int mode_0        =    round_mode&0x01;
            result.exponent   =    ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 255 : 254;
            result.mantissa   =    ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 0   : 0x7fffff;  
        }
        else{
            result.exponent     =  round_expo;
            result.mantissa     =  mant_short & last_23_binary;
        }
    }
    return result;
    }
    
uint32_t Fp32::lzd(uint32_t mantissa_in){
    uint32_t zero_nums;
    uint32_t zero_num_4;
    uint32_t zero_num_3;
    uint32_t zero_num_2;
    uint32_t zero_num_1;
    uint32_t zero_num_0;
    debug_printf("-------num_4-----------");
    zero_num_4      =   !detect_one(&mantissa_in,16);
    debug_printf("zero_num_4 is %u",zero_num_4);
    debug_printf("-------num_3-----------");
    zero_num_3      =   !detect_one(&mantissa_in,8);
    debug_printf("zero_num_3 is %u",zero_num_3);
    debug_printf("-------num_2-----------");
    zero_num_2      =   !detect_one(&mantissa_in,4);
    debug_printf("zero_num_2 is %u",zero_num_2);
    debug_printf("-------num_1-----------");
    zero_num_1      =   !detect_one(&mantissa_in,2);
    debug_printf("zero_num_1 is %u",zero_num_1);
    debug_printf("-------num_0-----------");
    zero_num_0      =   !detect_one(&mantissa_in,1);
    debug_printf("zero_num_0 is %u",zero_num_0);

    zero_nums       =   (zero_num_4<<4)+(zero_num_3<<3)+(zero_num_2<<2)+(zero_num_1<<1)+zero_num_0;
    // debug_printf("zero_nums is %u",zero_nums);

    return zero_nums;
}

uint32_t Fp32::detect_one(uint32_t* mantissa_in,uint32_t width)
{   
    uint32_t one_exist        =   0;
    uint64_t mantissa_record  =   *mantissa_in;
    while(width>0)
    {   
        // debug_printf("mantissa is         %b",*mantissa_in);
        // debug_printf("the_24_binary is    %b",the_24_binary);
        // debug_printf("mantissa_highest is %b",*mantissa_in&the_24_binary);
        one_exist            |=   (*mantissa_in&the_24_binary)   ?1:0;
        width                -=   1;
        *mantissa_in          <<=   1;
        // debug_printf("one_exist is %u",one_exist);

    }
    (*mantissa_in)            =   one_exist ?   mantissa_record:(*mantissa_in);

    return one_exist;
}

uint32_t Fp32::get_mask(uint32_t mantissa_in){
    uint32_t mask           =   0               ;
    uint32_t mani           =   0               ;
    uint32_t or_i           =   0               ;
    uint32_t top_binary     =   0               ;
    uint32_t cur_binary     =   the_24_binary   ;
    debug_printf("--------get_mask---------");
    debug_printf("mantissa_in is %b",mantissa_in);
    for(int i= 0 ; i < 24; i++) {
        or_i    =   0;
        for (int j = 0; j < i; j++)
        {
            top_binary    =    the_24_binary        ; 
            or_i          =    or_i || (top_binary & mantissa_in) ;
            top_binary    >>=  1                    ;
        }
        mani        =   ((!or_i) && (mantissa_in & cur_binary))? 1: 0  ;
        mask        =   mask << 1 + mani                               ;
        cur_binary >>=1;
    }
    return mask;
}

uint32_t Fp32::get_the_zero_nums(uint32_t mantissa_a,uint32_t mantissa_b, uint64_t a_expo_is_zero,uint64_t b_expo_is_zero,uint64_t mantissa)
{    
    debug_printf("---------------------------");
    debug_printf("------into lod-------------");
    debug_printf("---------------------------");
    bool     revise             =   0                                                                 ;
    uint32_t zero_nums_c        =   0                                                                 ;
    uint32_t zero_nums_unc      =   0                                                                 ;
    uint32_t zero_input         =   (a_expo_is_zero  & !b_expo_is_zero) ? mantissa_a : 
                                    (!a_expo_is_zero & b_expo_is_zero)  ? mantissa_b : last_24_binary ;
    uint32_t mask_of_input      =   0                                                                 ;
    uint64_t mantissa_mask      =   0                                                                 ;     
    debug_printf("zero_input is %b",zero_input)                                                       ;                            
    zero_nums_unc               =   lzd(zero_input)                                                   ;
    debug_printf("------into get_mask-------------");
    mask_of_input               =   get_mask(zero_input)                                              ;
    debug_printf("mask_of_in is %b",mask_of_input)                                                    ;
    mantissa_mask               =   mask_of_input << 24                                               ;
    revise                      =   (mantissa & mantissa_mask) ? 1 : 0                                ;
    uint32_t revise_int         =   revise                                                            ;
    debug_printf("zero_nums_unc is %u",zero_nums_unc)                                                 ;
    debug_printf("revise is %u",revise_int);

    // revise                      =   ((mantissa & mantissa_mask) & (!zero_nums_unc)) ? 1 : 0           ;
    zero_nums_c                 =   revise ? zero_nums_unc-1    : zero_nums_unc                         ; 
    debug_printf("zero_nums_c is %u",zero_nums_c);
    debug_printf("zero_nums_c is %b",zero_nums_c);

    return zero_nums_c;
}