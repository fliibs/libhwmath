#include "test.h"
#include "fp32.h"

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

Fp32 Fp32::operator+ (const Fp32 &rhs) {
        return add(rhs);
    }

void Fp32::print() {
    printf("value      : %12f\n", (float)*this);
    printf(" - sign    : %12ld\n",sign);
    printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
    printf(" - mantissa: %12ld,%64lb, %f\n",mantissa,mantissa+ (1 << 23),(float)(mantissa + (1 << 23))/(1<<23));
    printf("\n");
    }
    
Fp32 Fp32::add(const Fp32 &rhs){
    Fp32 result(0.0f);
    //--------------to determine 0, NAN and INF
    
    //-----------expand mant_a and mant_b to 48 bit
    bool a_expo_or   = exponent!=0               ;
    bool b_expo_or   = rhs.exponent!=0           ; 
    bool a_is_n0;
    bool b_is_n0;
    a_is_n0=(exponent!=0)||(mantissa!=0);
    b_is_n0=(rhs.exponent!=0)||(rhs.mantissa!=0);
    uint64_t mant_a_l;
    uint64_t mant_b_l;
    mant_a_l         = (((static_cast<uint32_t>(a_expo_or)) << 23) +mantissa)    <<23    ;
    mant_b_l         = (((static_cast<uint32_t>(b_expo_or)) << 23) +rhs.mantissa)<<23    ;
    debug_printf("mant_a_l is %lb",mant_a_l);
    debug_printf("mant_b_l is %lb",mant_b_l);

    //------------expo_1
    debug_printf("---------------------------");
    debug_printf("------into expo_1----------");
    debug_printf("---------------------------");
    bool a_big;
    uint32_t a_31    = (exponent     << 23) + mantissa                   ;
    uint32_t b_31    = (rhs.exponent << 23) + rhs.mantissa               ; 
    a_big            = (a_31>=b_31)   ? 1              : 0               ;
    debug_printf("a_big is %d",static_cast<int>(a_big))                  ;

    //------------sign_pre
    debug_printf("---------------------------");
    debug_printf("------into sign_pre--------");
    debug_printf("---------------------------");
    bool     same_sign  ;
    bool     sign_with_m;
    bool     sign_neg   ;


    same_sign   =   (rhs.sign == sign)                ;

    sign_with_m =   (!same_sign) && (!(a_big ^ sign)) ; //sign is the same as minu-subtr;
    
    sign_neg    =   (!same_sign) && (a_big ^ sign)    ; //sign is the opposite as minu-subtr;

    // sign_1      =   a_big ? sign : rhs.sign           ;

    debug_printf("same_sign is %d",static_cast<int>(same_sign))                   ;
    debug_printf("sign_with_m is %u",static_cast<int>(sign_with_m))               ;
    debug_printf("sign_neg is %u",static_cast<int>(sign_neg))                     ;
    
    //-----------mant_c and mant_s
    debug_printf("---------------------------");
    debug_printf("------into mant_c and mant_s---------");
    debug_printf("---------------------------");
    uint64_t mant_s      ;
    uint64_t mant_c      ;
    uint32_t minu_e      ;
    uint32_t subtr_e     ;
    bool     minu_exp_or ;
    bool     subtr_exp_or;
    
    minu_e      = a_big ?  exponent     : rhs.exponent      ;
    minu_exp_or = a_big ? a_expo_or     : b_expo_or         ;
    subtr_e     = a_big ?  rhs.exponent : exponent          ;
    subtr_exp_or= a_big ? b_expo_or     : a_expo_or         ;

    uint32_t expo_1  = minu_e                               ;
    uint32_t shift_l = minu_e - subtr_e + static_cast<uint32_t>(!minu_exp_or) - static_cast<uint32_t>(!subtr_exp_or) ;

    debug_printf("minu_expo_is_0 is %d",static_cast<uint32_t>(!minu_exp_or));
    debug_printf("subtr_expo_is_0 is %d",static_cast<uint32_t>(!minu_exp_or));

    debug_printf("expo_1 is %u",expo_1)  ;
    debug_printf("subr_e is %u",subtr_e) ; 
    debug_printf("shift_l is %u",shift_l);
    //--added
    bool shift_more_than_48 ;
    shift_more_than_48= shift_l >= 47 ? 1:0;
    shift_l     = shift_l > 48 ? 48                     : shift_l                ; 
    mant_s      = a_big        ? (mant_b_l >> shift_l)  : (mant_a_l >> shift_l)  ;
    // if(a_big){
    //     if(shift_more_than_48 && b_is_n0)
    //         mant_s=1;
    //     else
    //         mant_s=mant_b_l>>shift_l;
    // }
    // else{
    //     if(shift_more_than_48 && a_is_n0)
    //         mant_s=1;
    //     else
    //         mant_s=mant_a_l>>shift_l;
    // }
    if(a_big){
        if((!mant_s)&&(mant_b_l))
            mant_s=1;
        else
            mant_s=mant_b_l>>shift_l;
    }
    else{
        if(!mant_s&&mant_a_l)
            mant_s=1;
        else
            mant_s=mant_a_l>>shift_l;
    }
    if(a_big){
        debug_printf("mant_shift is b");
    }
    else{
        debug_printf("mant_shift is a");
    }

    if(a_big && (sign||sign_neg)){
        mant_c = 1-mant_a_l ;
        debug_printf("mant_comp is 1-a");
    } 
    else{
        if(a_big && (!sign))
        {
            mant_c = mant_a_l;
            debug_printf("mant_comp is a");
        }
        else{
            if((!a_big)&&((rhs.sign)||sign_neg)){
                mant_c = 1 - mant_b_l;
                debug_printf("mant_comp is 1-b");
            }
            else{
                mant_c = mant_b_l;
                debug_printf("mant_comp is b");
            }
        }
    }
    uint64_t debug_refe_c=0x7fffff800000;

    debug_printf("mant_s is %lu",mant_s)                    ;
    debug_printf("mant_s is %lb",mant_s)                    ;
    debug_printf("mant_c is %lu",mant_c)                    ;
    debug_printf("mant_c is %lb",mant_c)                    ;
    // debug_printf("mant_b is %lb",mant_b_l)                  ;
    debug_printf("refe_c is %lb",debug_refe_c)              ;
    debug_printf("shift_more_than_48 is %d",static_cast<int>(shift_more_than_48));


    //-----------mant_a and mant_b and mant_2
    debug_printf("---------------------------");
    debug_printf("------into mant_2---------");
    debug_printf("---------------------------");
    uint64_t mant_a     ;
    uint64_t mant_b     ;
    uint64_t mant_1     ;

    mant_a      =   mant_s                                      ;
    mant_b      =   same_sign ? (a_big ? mant_a_l :mant_b_l)    :
                    mant_c                                      ;

    mant_1      =   mant_a    + mant_b                          ;
    
    bool mant_carry     ;
    bool mant_1_sign    ;
    bool sign_2         ;
    
    uint64_t mant_mid   ;
    uint32_t zero_nums  ;

    uint64_t mant_2     ;
    uint64_t expo_2     ;
    
    mant_1_sign = mant_1 & mantissa_49_bit                      ; 
    sign_2      = sign_neg             ? (!mant_1_sign) : mant_1_sign     ;
    mant_mid    = (sign_2 || sign_neg) ? (1-mant_1)     : mant_1          ;

    //leading zero detects,actually it can be detecting the leading 1 nums in mant_1
    mant_carry  = mant_mid & mantissa_48_bit                    ;
    mant_2      = mant_carry ?  mant_mid >> 1 : mant_mid        ;
    expo_2      = mant_carry ?  expo_1 + 1    : expo_1          ;

    zero_nums=get_the_zero_nums_add(mant_2);
    debug_printf("the zero_nums is %d",zero_nums);


    if(expo_2<=zero_nums){
        debug_printf("into expo<=zero_nums");
        uint32_t right_shift;
        right_shift= expo_2 ? (expo_2-1):expo_2; 
        mant_2 = mant_2<<right_shift;
        expo_2 = 0                 ;
    }
    else
    {
        debug_printf("into expo>zero_nums");
        mant_2 = mant_2 <<zero_nums;
        expo_2 = expo_2 - zero_nums;
    }

    
    debug_printf("mant_1 is %lb"     ,mant_1)                           ;
    debug_printf("refe_c is %lb"    ,debug_refe_c)                      ;
    debug_printf("mant_1_sign is %d",static_cast<int>(mant_1_sign))     ;
    debug_printf("sign_2 is %d"     ,static_cast<int>(sign_2))          ;
    debug_printf("mant_mid is %lb"  ,mant_mid)                          ;
    debug_printf("refe_c   is %lb"  ,debug_refe_c)                      ;

    debug_printf("mant_carry is %d",static_cast<int>(mant_carry))       ;

    debug_printf("mant_2 is %lb"    ,mant_2)                            ;
    debug_printf("refe_c is %lb"    ,debug_refe_c)                      ;

    debug_printf("expo_2 is %u"     ,expo_2)                            ;
    debug_printf("---------------------------");
    debug_printf("------into rounding--------");
    debug_printf("---------------------------");

    bool sign_c;
    sign_c  = same_sign ? sign : sign_2 ;
    bool     g          ;
    bool     r          ;
    bool     s          ;
    bool     s_bit_r    ;
    bool     rnd_carry  ;
    
    uint32_t expo_3     ;
    uint32_t mant_3     ;
    uint64_t mant_rnd   ;

    s_bit_r = mant_mid & 0x01                      ;                                                                           
    g       = mant_2 & the_24_binary               ;
    r       = mant_2 & the_23_binary               ;
    s       = (mant_2 & last_22_binary) || s_bit_r ;

    rnd_carry  =   (round_mode==3) ? ((r && (g||s))    ?1:0) : 
                    (round_mode==2) ? ((!sign_c) &&(a_is_n0) && (b_is_n0) &&(r||s||(expo_2==0 && mant_2==0) ?1:0)) :
                    (round_mode==1) ? (sign_c    &&(a_is_n0) && (b_is_n0) &&(r||s||(expo_2==0 && mant_2==0) ?1:0)) :
                    0;
    
    
    mant_rnd    =   (mant_2>>23) + rnd_carry                 ;
    expo_3      =   (mant_rnd&the_25_binary)||((!expo_2)&&(mant_rnd&the_24_binary)) ? (expo_2 + 1)                    : expo_2                     ;
    mant_3      =   (mant_rnd&the_25_binary)                                        ? ((mant_rnd>>1) & last_23_binary): (mant_rnd & last_23_binary);  
    
    debug_printf("rnd_mode is %d",  round_mode);
    debug_printf("g is %d"              ,static_cast<int>(g))           ;
    debug_printf("r is %d"              ,static_cast<int>(r))           ;
    debug_printf("s is %d"              ,static_cast<int>(s))           ;
    debug_printf("a_is_n0 is %d"        ,static_cast<int>(a_is_n0))     ;
    debug_printf("b_is_n0 is %d"        ,static_cast<int>(b_is_n0))     ;
    debug_printf("b_is_n0 is %d"        ,static_cast<int>(b_is_n0))     ;
    debug_printf("l_con is %d"      ,static_cast<int>(r||s||(expo_2==0)))   ;
    debug_printf("rnd carry is %d"      ,static_cast<int>(rnd_carry))   ;
    debug_printf("mant_rnd is %lb"      ,mant_rnd)                      ;
    debug_printf("mant_rnd_25 is %lb"   ,mant_rnd&the_25_binary)        ;
    debug_printf("expo_3 is %u"         ,expo_3)                        ;
    debug_printf("mant_3 is %lb"        ,mant_3)                        ;

    //------masking 
    debug_printf("---------------------------");
    debug_printf("------into masking---------");
    debug_printf("---------------------------");
    bool    overflow   ;

    bool    a_is_0     ;
    bool    b_is_0     ;
    bool    a_is_inf   ;
    bool    b_is_inf   ;
    bool    a_is_nan   ;
    bool    b_is_nan   ;

    bool    r_is_nan   ;
    bool    is_inf_nan ;
    uint32_t mant_unsel;
    uint32_t mant_nan  ;
    bool     sign_unsel;
    bool     sign_nan  ;


    overflow    = expo_3>=255                                ;
    a_is_0      = (exponent==0)         && (mantissa==0)     ;
    b_is_0      = (rhs.exponent==0)     && (rhs.mantissa==0) ;
    a_is_inf    = (exponent==255)       && (mantissa==0)     ;
    b_is_inf    = (rhs.exponent==255)   && (rhs.mantissa==0) ;
    a_is_nan    = (exponent==255)       && (mantissa!=0)     ;
    b_is_nan    = (rhs.exponent==255)   && (rhs.mantissa!=0) ;

    r_is_nan    = a_is_nan | b_is_nan                        ;
    is_inf_nan  = a_is_nan | b_is_nan | a_is_inf | b_is_inf  ;
    
    mant_unsel  = ((exponent<<1)+(mantissa!=0) >= (rhs.exponent<<1)+(rhs.mantissa!=0) ) ? ((1<<22)+(mantissa & last_22_binary)) :((1<<22)+(rhs.mantissa & last_22_binary));
    mant_nan    = r_is_nan                   ? mant_unsel                            : 0                                       ;

    sign_unsel  = ((exponent<<1)+(mantissa!=0) >= (rhs.exponent<<1)+(rhs.mantissa!=0) ) ?  sign                                  : rhs.sign                                ;
    result.sign = r_is_nan                   ? sign_unsel                            : sign_c                                  ;

    debug_printf("mant_unsel is %b",mant_unsel);

    bool    mode_1  ;
    bool    mode_0 ;

    mode_1      = round_mode & 0x02;
    mode_0      = round_mode & 0x01;

    
    if(is_inf_nan){
        debug_printf("result is inf or nan");
        result.exponent = 255               ;
        result.mantissa = mant_nan          ;
    }
    else{
        debug_printf("into regular")        ;
        if(overflow){
            debug_printf("overflow")        ;
            result.exponent   = ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 255 : 254         ;
            result.mantissa   = ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 0   : 0x7fffff    ;  
        }
        else{
            debug_printf("not overflow")    ;
            result.exponent   = expo_3      ;
            result.mantissa   = mant_3      ;
            debug_printf("mant_3 is %lb",mant_3);
            debug_printf("mant_r is %lb",result.mantissa);
        }
    }
    debug_printf("sign_c is %d",static_cast<int>(sign_c ));
    return result;
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

    a_mant_total             =   ((1-a_expo_is_zero)<<23)+mantissa;
    b_mant_total             =   ((1-b_expo_is_zero)<<23)+rhs.mantissa;
    temp_mant                =   a_mant_total*b_mant_total;
    temp_mant_last_47        =   temp_mant&mantissa_get_47_bit;
    zero_nums                =   get_the_zero_nums(&temp_mant_last_47);
    bit_s_record             =   0;

    uint64_t temp_mant_l_48=temp_mant&mantissa_48_bit;
    uint64_t temp_mant_ref=0xffffffffffff;
    debug_printf("temp_mant is    %lb",temp_mant);
    debug_printf("temp_mant_48 is %lb",temp_mant_l_48);
    debug_printf("temp_mant_rf is %lb",temp_mant_ref);
    debug_printf("temp_mant_47 is %lb",temp_mant_last_47);
    debug_printf("temp_mant is    %lu",temp_mant);
    debug_printf("temp_mant_47 is %lu",temp_mant_last_47);

    if((temp_mant&mantissa_48_bit)&&(!(temp_expo&exponent_sign))){                                                             //1x.xxxxxxx
        debug_printf("into shift right");
        man_expo             =   temp_expo+1;
        bit_s_record         =   temp_mant&0x01;
        mant_long            =   temp_mant>>1;
    }
    else{
        debug_printf("not into shift right");
        uint32_t expo_sign=temp_expo&exponent_sign;
        debug_printf("exponent sign is %u",expo_sign);
        debug_printf("zero_num is %u",zero_nums);
        uint64_t temp_mant_l_47=temp_mant_last_47&mantissa_47_bit;
        debug_printf("temp_mant's 47 bit is %lb",temp_mant_l_47);
        debug_printf("temp_mant's 47 bit is %lu",temp_mant_l_47);

        if(!(temp_expo&exponent_sign) && ((temp_expo&exponent_num)>zero_nums) && (temp_mant_last_47&mantissa_47_bit)){         //0.xxxxxx and normailized
            debug_printf("into 0.xxxx and normalized");
            man_expo         =   temp_expo-zero_nums;
            mant_long        =   temp_mant_last_47;
        }
        else{                                                                                                                  //0.xxxxxxx and denormailized
            debug_printf("into 0.xxxx and denormalized");
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
                    shift_left  =   ((1-temp_expo)>63)?63:1-temp_expo;
                    mant_long   =   temp_mant>>shift_left;
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
    
uint32_t Fp32::get_the_zero_nums(uint64_t *mantissa_in){
    uint32_t zero_nums;
    uint32_t zero_num_5;
    uint32_t zero_num_4;
    uint32_t zero_num_3;
    uint32_t zero_num_2;
    uint32_t zero_num_1;
    uint32_t zero_num_0;
    zero_num_5      =   !detect_one(&*mantissa_in,32);
    zero_num_4      =   !detect_one(&*mantissa_in,16);
    zero_num_3      =   !detect_one(&*mantissa_in,8);
    zero_num_2      =   !detect_one(&*mantissa_in,4);
    zero_num_1      =   !detect_one(&*mantissa_in,2);
    zero_num_0      =   !detect_one(&*mantissa_in,1);
    zero_nums   =   (zero_num_5<<5)+(zero_num_4<<4)+(zero_num_3<<3)+(zero_num_2<<2)+(zero_num_1<<1)+zero_num_0;
    return zero_nums;
}

uint32_t Fp32::detect_one(uint64_t *mantissa_in,uint32_t width)
{   
    uint32_t one_exist        =   0;
    uint64_t mantissa_record  =   *mantissa_in;
    while(width>0)
    {
        one_exist            |=   ((*mantissa_in)&mantissa_47_bit)   ?1:0;
        width                -=   1;
        (*mantissa_in)      <<=   1;
    }
    (*mantissa_in)            =   one_exist ?   mantissa_record:(*mantissa_in);
    return one_exist;
}

uint32_t Fp32::get_the_zero_nums_add(uint64_t mantissa_in){
    uint32_t zero_nums;
    uint32_t zero_num_5;
    uint32_t zero_num_4;
    uint32_t zero_num_3;
    uint32_t zero_num_2;
    uint32_t zero_num_1;
    uint32_t zero_num_0;
    zero_num_5      =   !detect_one(&mantissa_in,32);
    zero_num_4      =   !detect_one(&mantissa_in,16);
    zero_num_3      =   !detect_one(&mantissa_in,8);
    zero_num_2      =   !detect_one(&mantissa_in,4);
    zero_num_1      =   !detect_one(&mantissa_in,2);
    zero_num_0      =   !detect_one(&mantissa_in,1);
    zero_nums   =   (zero_num_5<<5)+(zero_num_4<<4)+(zero_num_3<<3)+(zero_num_2<<2)+(zero_num_1<<1)+zero_num_0;
    return zero_nums;
}

void Fp32::debug_printf(const char* cmd, ...){
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
}
else{
    // printf("debug is %d\n",debug);  
}
}