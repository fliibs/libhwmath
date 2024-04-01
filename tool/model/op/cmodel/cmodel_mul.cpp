#include "cmodel_mul.h"
std::array<int,5> cmodel_mul::mul(const FpBase& a,const FpBase& b,const FpBase& c,const int& rnd_mode,FpBase *result)
{
    VariablesTable.clear();
    std::array<int,5> arr={0,0,0,0,0}; 
    uint32_t    a_expo        = a.expo;
    uint32_t    b_expo        = b.expo;
    mp::cpp_int a_mant        = a.mant;
    mp::cpp_int b_mant        = b.mant;

    int         res_expo_w    = (*result).expo_w;
    int         res_mant_w    = (*result).mant_w;
    info1.debug_printf("--------------------------------");
    info1.debug_printf("-----------into c model---------");
    info1.debug_printf("--------------------------------");

    bool a_expo_is_0;
    bool b_expo_is_0;
    a_expo_is_0  = static_cast<bool>(a_expo==0);
    b_expo_is_0  = static_cast<bool>(b_expo==0);
    
    bool a_expo_is_max;
    bool b_expo_is_max;
    a_expo_is_max = expo_whe_max(a_expo,a.expo_w);
    b_expo_is_max = expo_whe_max(b_expo,b.expo_w);
    
    bool a_mant_is_0;
    bool b_mant_is_0;
    mp::cpp_int zero_cpp=0;
    a_mant_is_0   = static_cast<bool>(a_mant==zero_cpp);
    b_mant_is_0   = static_cast<bool>(b_mant==zero_cpp);

    bool a_is_0;
    bool b_is_0;
    bool a_is_nan;
    bool b_is_nan;
    bool a_is_inf;
    bool b_is_inf;
    a_is_0       = (a_expo_is_0)  &&(a_mant_is_0);
    b_is_0       = (b_expo_is_0)  &&(b_mant_is_0);
    a_is_inf     = (a_expo_is_max)&&(a_mant_is_0);
    b_is_inf     = (b_expo_is_max)&&(b_mant_is_0);
    a_is_nan     = (a_expo_is_max)&&(!a_mant_is_0);
    b_is_nan     = (b_expo_is_max)&&(!b_mant_is_0);

    bool a_is_q;
    bool b_is_q;
    a_is_q             = static_cast<bool>(a.mant>>(a.mant_w-1));
    b_is_q             = static_cast<bool>(b.mant>>(b.mant_w-1));

    bool a_is_s_nan;
    bool b_is_s_nan;
    a_is_s_nan      = (!a_is_q) && a_is_nan;
    b_is_s_nan      = (!b_is_q) && b_is_nan;

    bool r_is_nan;
    bool in_is_nan;
    bool is_inf_nan;
    bool r_is_0nan;

    r_is_0nan       = (a_is_0 & b_is_inf) | (b_is_0 & a_is_inf);
    in_is_nan       = a_is_nan  || b_is_nan;
    r_is_nan        = in_is_nan || r_is_0nan;
    is_inf_nan      = in_is_nan || a_is_inf || b_is_inf;
    
    bool  a_is_nor;
    bool  b_is_nor;
    a_is_nor        = !a_is_inf && !a_is_0 && !a_is_nan;
    b_is_nor        = !b_is_inf && !b_is_0 && !b_is_nan;

    bool a_is_n0;
    bool b_is_n0;
    a_is_n0=!a_is_0;
    b_is_n0=!b_is_0;

    bool status_nv=a_is_s_nan || b_is_s_nan || r_is_0nan;
    bool r_is0nan =r_is_0nan;
    bool r_isnan  =r_is_nan;
    echo(a_expo);
    echo(b_expo);
    echo(a_mant);
    echo(b_mant);
    echo(a_is_n0);
    echo(b_is_n0);
    echo(a_is_nan);
    echo(b_is_nan);
    echo(a_is_q);
    echo(b_is_q);
    echo(r_is0nan);
    echo(in_is_nan);
    echo(r_isnan);
    echo(is_inf_nan);
    echo(a_is_nor);
    echo(b_is_nor);
    echo(status_nv);
    int rnd=rnd_mode;
    echo(a_is_s_nan);
    echo(b_is_s_nan);
    echo(r_is_0nan);
    echo(rnd);

    //--------------generate sign
    (*result).sign =  a.sign ^ b.sign;
    info1.debug_printf("result sign is %d",static_cast<int>((*result).sign));
    
    //-------------temp_expo
    uint32_t expo_1;
    uint32_t res_expo_max;
    uint32_t a_expo_max;
    uint32_t b_expo_max;
    a_expo_max    = set_expo_max(a.expo_w);
    b_expo_max    = set_expo_max(b.expo_w);
    res_expo_max  = set_expo_max((*result).expo_w);
    expo_1        = a_expo + static_cast<uint32_t>(a_expo_is_0) - (a_expo_max/2) 
                    + b_expo + static_cast<uint32_t>(b_expo_is_0) - (b_expo_max/2) 
                    +(res_expo_max/2); 

    //--------------temp_expo debug
    info1.debug_printf("expo_1      is %u\n",expo_1);

    //-------------generate mantissa
    info1.debug_printf("--------------------------------");
    info1.debug_printf("-------generate mantissa--------");
    info1.debug_printf("--------------------------------");
    mp::cpp_int a_mant_total;
    mp::cpp_int b_mant_total;
    a_mant_total         = ((static_cast<mp::cpp_int>(1-a_expo_is_0))<<a.mant_w) + a_mant;
    b_mant_total         = ((static_cast<mp::cpp_int>(1-b_expo_is_0))<<b.mant_w) + b_mant;

    mp::cpp_int mant_1;
    mp::cpp_int mant_1_str;    //store mant_1 
    int mant_av_nums;          //available mant_nums;in fp32 * fp32,it's 47
    int mant_exd_nums;         //bit that exceeding the mant_av_nums;in fp32*32,it's 48
    mp::cpp_int mant_av_bits;
    mp::cpp_int mant_exd_top_bit;

    mant_1               = a_mant_total * b_mant_total;
    
    info1.debug_printf("a_mant_total:\n");
    info1.print_cpp_int(a_mant_total,"b");
    info1.debug_printf("b_mant_total:\n");
    info1.print_cpp_int(b_mant_total,"b");
    info1.debug_printf("mant_1:\n");
    info1.print_cpp_int(mant_1,"b");

    bool sign_1 = (*result).sign;
    echo(sign_1);
    echo(expo_1);
    echo(mant_1);

    mant_1_str           = mant_1;
    mant_av_nums         = 1+a.mant_w+b.mant_w;             //
    mant_av_bits         = get_mant_av_bits(mant_av_nums);  //the avail mant bits of mant all set to one; in fp32 * fp32 it's 47 bits all equaling one 
    mant_exd_nums        = mant_av_nums+1;                  
    mant_exd_top_bit     = get_mant_av_top_bit(mant_exd_nums);
    
    info1.debug_printf("mant_av_nums is %d",mant_av_nums);
    int zero_nums;
    zero_nums            = get_zero_nums(&mant_1,mant_av_nums);
    
    //-------------mantissa debug
    info1.debug_printf("mant_1 is::\n");
    info1.print_cpp_int(mant_1_str,"b");
    
    //-----------------generating the final exponent results
    info1.debug_printf("--------------------------------");
    info1.debug_printf("-------handle subnormals--------");
    info1.debug_printf("--------------------------------");
    //------output
    mp::cpp_int s_bit_rcd;          //record s bits exceeding res_mant_w;
    mp::cpp_int mant_2;
    uint32_t    expo_2;
    bool        underflow  =false;
    bool        inexact_sft=false;
    uint32_t expo_sign_bit; //resul expo sign bit
    expo_sign_bit = get_expo_sign_bit((*result).expo_w);
    uint32_t expo_exd_max;  //result expo width + 1 all equaling one
    expo_exd_max  = set_expo_max(((*result).expo_w)+1);


    //------if conditions
    bool con_expo_sign_0;
    bool con_mant_exd;
    con_expo_sign_0 = !(expo_1 & expo_sign_bit);     //the expo_1 sign is zero
    con_mant_exd    = ((mant_1_str & mant_exd_top_bit)!=0);   //the mant exceed mant_av_nums
    uint32_t expo_nums;
    expo_nums      =  expo_exd_max & expo_1;         

    //-------shift relevant
    uint32_t l_shift; // left shift nums
    uint32_t r_shift; // right shift nums

    if((con_mant_exd) && (con_expo_sign_0)){
        info1.debug_printf("into shift right");
        expo_2      = expo_1+1;
        mant_2      = mant_1_str>>1;
        s_bit_rcd   = mant_1_str&0x01;
        inexact_sft = static_cast<bool>(s_bit_rcd);
    }
    else{
        info1.debug_printf("not into shift right");   
        if(con_expo_sign_0 && (expo_nums>zero_nums) && (mant_1_str & mant_av_bits)){
            info1.debug_printf("into 0.xxxx and normalized");
            expo_2   = expo_1 - zero_nums;
            mant_2   = mant_1;
        }
        else{
            // underflow=true;
            info1.debug_printf("into 0.xxxx and denormalized");
            expo_2   = 0;
            if(con_expo_sign_0 && expo_nums) {
                l_shift     = expo_1-1;
                mant_2      = mant_1_str<<l_shift;
            }
            else{
                r_shift     = 1-expo_1;
                mant_2      = mant_1_str>>r_shift;
                mp::cpp_int b_valid;
                b_valid     = get_mant_av_bits(r_shift);
                s_bit_rcd   = (mant_1_str&b_valid);
                inexact_sft = static_cast<bool>(s_bit_rcd);
                // underflow=true;
            }
        }
    }
    echo(r_shift);
    echo(l_shift);
    mp::cpp_int zero_nums_c;
    echo(zero_nums_c)
    mp::cpp_int mant_2_l_zn=mant_1;
    echo(mant_2_l_zn);
    echo(inexact_sft);


    //---------------------handling subnormals debug
    info1.debug_printf("expo_sign_bit is %b\n",expo_sign_bit);
    info1.debug_printf("expo_exd_max  is %b\n",expo_exd_max);
    info1.debug_printf("expo_1        is %b\n",expo_1);
    info1.debug_printf("expo_nums     is %b\n",expo_nums);
    info1.debug_printf("zero_nums     is %b\n",zero_nums);
    info1.debug_printf("con_expo_s_0  is %b\n",con_expo_sign_0);

    info1.debug_printf("expo_2        is %b\n",expo_2);
    info1.debug_printf("mant_2 is: \n");
    info1.print_cpp_int(mant_2,"b");
    
    //--------------rounding
    info1.debug_printf("---------------------------");
    info1.debug_printf("------into rounding--------");
    info1.debug_printf("---------------------------");
    //------output 
    mp::cpp_int mant_3;
    uint32_t    expo_3;
    bool        inexact_rnd;
    //-----internal 
    mp::cpp_int mant_rnd;

    bool g;
    bool r;
    bool s;
    bool mant_carry;

    // mp::cpp_int mant_g_r_bits; //mant with all available and r bits setting to 1;
    mp::cpp_int mant_g_bit;//mant mask with g bit
    mp::cpp_int mant_r_bit;//mant mask with r bit
    mp::cpp_int mant_s_mask; //mant mask to get the whole s bits

    mant_s_mask   = get_mant_av_bits((mant_av_nums-(*result).mant_w-2));

    // mant_g_r_bits = get_rnd_g_r(mant_av_nums,(*result).mant_w);
    mant_g_bit    = get_rnd_g(mant_av_nums,(*result).mant_w);
    mant_r_bit    = get_rnd_r(mant_av_nums,(*result).mant_w);

    g             = static_cast<bool>((mant_g_bit&mant_2)!=0);
    r             = static_cast<bool>((mant_r_bit&mant_2)!=0);
    s             = static_cast<bool>(s_bit_rcd | (mant_2 & mant_s_mask));
    inexact_rnd   = r || s;
    mant_carry    = (rnd_mode==3)? ((r &&( g || s))? 1 : 0):
                    (rnd_mode==2)? ((!(*result).sign && (!a_is_0) && (!b_is_0) && (r||s)) ? 1 : 0):
                    (rnd_mode==1)? (((*result).sign  && (!a_is_0) && (!b_is_0) && (r||s)) ? 1 : 0):
                    0;
    int rnd_shift = (mant_av_nums-(*result).mant_w-1);

    mant_rnd      = (mant_2>>rnd_shift) + static_cast<mp::cpp_int>(mant_carry);
    mp::cpp_int int_2_mask;
    mp::cpp_int int_1_mask;
    mp::cpp_int res_mant_mask;
    int_2_mask    = get_mant_av_top_bit((*result).mant_w+2);
    int_1_mask    = get_mant_av_top_bit((*result).mant_w+1);
    res_mant_mask = get_mant_av_bits((*result).mant_w);


    expo_3        = (mant_rnd & int_2_mask) || ((!expo_2) && (mant_rnd & int_1_mask)) ? (expo_2 + 1) : expo_2 ;
    
    mp::cpp_int mant_rest_left;
    mp::cpp_int mant_rest;
    mant_rest_left=(mant_rnd>>1) & res_mant_mask;
    mant_rest     = mant_rnd & res_mant_mask;
    mant_3        = (mant_rnd & int_2_mask) ? mant_rest_left : mant_rest;
    
    info1.debug_printf("mant_2:");
    info1.print_cpp_int(mant_2,"b");
    info1.debug_printf("mant_rnd:");
    info1.print_cpp_int(mant_rnd,"b");
    info1.debug_printf("int_2_mask:");
    info1.print_cpp_int(int_2_mask,"b");
    info1.debug_printf("int_1_mask:");
    info1.print_cpp_int(int_1_mask,"b");
    info1.debug_printf("res_mant_mask:");
    info1.print_cpp_int(res_mant_mask,"b");
    echo(inexact_rnd);
    echo(expo_3);
    echo(mant_3);
    
    bool underflow_mask;
    underflow_mask = r||s;
    //------------generating output,it seems like inf and nan can't be optimized into the current process.
    info1.debug_printf("---------------------------");
    info1.debug_printf("------into masking---------");
    info1.debug_printf("---------------------------");
    bool overflow;
    mp::cpp_int mant_unsel;
    mp::cpp_int mant_nan;
    bool sign_nan;
    bool sign_unsel;
    
    mp::cpp_int a_mant_nan;//get the a represent nan;
    mp::cpp_int b_mant_nan;//get the b represent nan;

    int expo_of;
    expo_of = std::ceil(std::pow(2,(*result).expo_w))-1;
    overflow        = (expo_3>=expo_of)||(expo_2>=expo_of);///!!!!!

    a_mant_nan      = get_nan(a.mant_w,(*result).mant_w,a_mant);
    b_mant_nan      = get_nan(b.mant_w,(*result).mant_w,b_mant);

    bool  inher_a;
    inher_a         = ((a_expo & a_expo_max)+a_is_nan) >= ((b_expo & b_expo_max)+b_is_nan); //don't use add in rtl expand 1 bit

    mant_unsel      = inher_a  ? (a_mant_nan) :(b_mant_nan);
    mant_nan        = r_is_nan ? mant_unsel : 0;

    sign_unsel      = inher_a  ? a.sign : b.sign;
    sign_nan        = r_is_nan ? (r_is_0nan | sign_unsel) : (*result).sign;

    if(is_inf_nan){
        (*result).sign = sign_nan;
        (*result).expo = res_expo_max;
        (*result).mant = static_cast<uint64_t>(mant_nan);
    }
    else{
        if(overflow){
            int mode_1  = rnd_mode & 0x02;
            int mode_0  = rnd_mode & 0x01;
            (*result).expo = ((mode_1 && (!(*result).sign)) || (mode_0 && (*result).sign)) ? res_expo_max : (res_expo_max-1);
            (*result).mant = ((mode_1 && (!(*result).sign)) || (mode_0 && (*result).sign)) ? 0   : static_cast<uint64_t>(res_mant_mask);  
        }
        else{
            (*result).expo = expo_3;
            (*result).mant = static_cast<uint64_t>(mant_3);
        }
    }
    echo(overflow);
    echo(sign_nan);

    mp::cpp_int mant_4=mant_nan;
    echo(mant_4);

    bool res_sign = (*result).sign;
    echo(res_sign);
    uint32_t res_expo = (*result).expo;
    echo(res_expo);
    uint64_t res_mant = (*result).mant;
    echo(res_mant);

    info1.debug_printf("---------------------------");
    info1.debug_printf("------into exceptions------");
    info1.debug_printf("---------------------------");

    arr[4]          = a_is_s_nan || b_is_s_nan || r_is_0nan;
    arr[3]          = 0;
    arr[2]          = overflow && (!is_inf_nan);
    // arr[1]          = underflow && (!a_is_0) && (!b_is_0) && underflow_mask;
    arr[1]          = (!a_is_0)&&(!b_is_0)&&(res_expo==0)&&underflow_mask;
    bool  inexact;
    inexact         = inexact_rnd || inexact_sft;
    bool  inexact_of;
    inexact_of      = a_is_nor && b_is_nor && overflow;
    arr[0]          = (inexact && !r_is_nan) || inexact_of;
    std::array<int,5> status;
    status=arr;
    echo(status[4]); 
    echo(status[3]); 
    echo(status[2]); 
    echo(status[1]); 
    echo(status[0]); 
    return arr;

}