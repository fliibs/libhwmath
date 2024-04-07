#include "cmodel_fma.h"

std::array<int,5> cmodel_fma::fma(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res){
    std::array<int,5> status;
    //---------
    //classify
    //---------
    classify(a,b,c);

    //-----------
    //Mul a and b
    //-----------
    info1.debug_printf("--------------------------");
    info1.debug_printf("------Mul a and b---------");
    info1.debug_printf("--------------------------");
    Mul_res mul_res1=fma_mul(a,b,*res);

    //------------------------ 
    //shift and inversion of a 
    //------------------------
    info1.debug_printf("----------------------------------------");
    info1.debug_printf("-------shift and inversion of a---------");
    info1.debug_printf("----------------------------------------");
    Sft_Inv_res sft_inv_res1;
    sft_inv_res1=fma_sft_inv(mul_res1,c,a.mant_w,b.mant_w);

    //------------------------ 
    //get st1
    //------------------------
    info1.debug_printf("-----------------------");
    info1.debug_printf("-------get st1---------");
    info1.debug_printf("-----------------------");
    bool st1;
    st1=fma_get_st1(sft_inv_res1);

    //-------------------------------------------  
    //-----------sign detection 
    //-------------------------------------------  
    info1.debug_printf("---------------------------");
    info1.debug_printf("-------sign detect---------");
    info1.debug_printf("---------------------------");
    int expo_width_dif = a.expo_w + b.expo_w - c.expo_w        ;
    expo_width_dif     = expo_width_dif<0 ? 0 : expo_width_dif ;
    Sign_det_res sign_det_res1;
    sign_det_res1      = fma_sign_det(mul_res1,sft_inv_res1,expo_width_dif,c.sign);

    //---------------------------------------------------------------------------------
    //-----------csa add.one 48-bit adder + one 2-bit adder + two 24-bit adders + 
    //---------------------------------------------------------------------------------
    info1.debug_printf("-----------------------");
    info1.debug_printf("-------csa add---------");
    info1.debug_printf("-----------------------");
    int mul_mant_w = 2 + a.mant_w + b.mant_w;

    Csa_add_in  csa_add_in1 ;
    csa_add_in1.c_mant_s_l = (sft_inv_res1.c_mant_s<<1) + static_cast<mp::cpp_int>(!st1);
    csa_add_in1.mul_mant_l = (mul_res1.mant_mul<<1)     + static_cast<mp::cpp_int>(sft_inv_res1.sub) ;

    mp::cpp_int mant_add    ;
    mant_add       = fma_csa_add(csa_add_in1,mul_mant_w,c.mant_w);

    info1.debug_printf("----------------------------------");
    info1.debug_printf("-------csa add complement---------");
    info1.debug_printf("----------------------------------");
    mp::cpp_int mant_add_cpl;
    mant_add_cpl   = fma_cas_add_cpl(csa_add_in1,mul_mant_w,c.mant_w);

    //-------------------------------------------  
    //-----------loa,parallel with the adder
    //------------------------------------------- 
    //-----------get the adder leading zeros,only depanding on the p g a and d and sub
    //-----------can get the leading zero of the higher bits and the lower bits seperately
    //-----------has two leading zero detector for normal and converted ones 
    //-----------the same for csa add result and csa add complement result ,select one according to complement
    LOA_res loa_res1;
    int     loa_width;
    loa_width = 1+c.mant_w+2+2+a.mant_w+b.mant_w+1;
    loa_res1  = fma_loa(csa_add_in1,loa_width);
    

    (*res).sign = a.sign;
    (*res).expo = a.expo;
    (*res).mant = a.mant;
    return status ;

}

//fma sub model
void cmodel_fma::classify(const FpBase& a, const FpBase& b,  const FpBase& c){
    bool a_expo_is_zero = (a.expo==0)   ;
    bool b_expo_is_zero = (b.expo==0)   ;
    bool c_expo_is_zero = (c.expo==0)   ;

    uint32_t a_expo_max;
    uint32_t b_expo_max;
    uint32_t c_expo_max;

    a_expo_max          = set_expo_max(a.expo_w);
    b_expo_max          = set_expo_max(b.expo_w);
    c_expo_max          = set_expo_max(c.expo_w);

    bool a_expo_is_ff   = (a.expo==a_expo_max) ;
    bool b_expo_is_ff   = (b.expo==a_expo_max) ;
    bool c_expo_is_ff   = (c.expo==a_expo_max) ;
    bool a_mant_is_00   = (a.mant==0)          ;
    bool b_mant_is_00   = (b.mant==0)          ;
    bool c_mant_is_00   = (c.mant==0)          ;


    fpclass1.a_is_0         = (a_expo_is_zero && a_mant_is_00)      ;
    fpclass1.b_is_0         = (b_expo_is_zero && b_mant_is_00)      ;
    fpclass1.c_is_0         = (c_expo_is_zero && c_mant_is_00)      ;
    fpclass1.a_is_nan       = (a_expo_is_ff   && (!a_mant_is_00))   ;
    fpclass1.b_is_nan       = (b_expo_is_ff   && (!b_mant_is_00))   ;
    fpclass1.c_is_nan       = (c_expo_is_ff   && (!c_mant_is_00))   ;
    fpclass1.a_is_inf       = (a_expo_is_ff   && a_mant_is_00)      ;
    fpclass1.b_is_inf       = (b_expo_is_ff   && b_mant_is_00)      ;
    fpclass1.c_is_inf       = (c_expo_is_ff   && c_mant_is_00)      ;

    fpclass1.a_expo_is_0    = (a.expo==0)                           ;
    fpclass1.b_expo_is_0    = (b.expo==0)                           ;
    fpclass1.c_expo_is_0    = (c.expo==0)                           ;
}

Mul_res cmodel_fma::fma_mul(const FpBase& a, const FpBase& b, const FpBase& res){
    Mul_res res_mul;
    //mant
    uint64_t a_mant_in;
    uint64_t b_mant_in;
    a_mant_in   = ((1-fpclass1.a_expo_is_0)<<a.mant_w)+a.mant;
    b_mant_in   = ((1-fpclass1.b_expo_is_0)<<b.mant_w)+b.mant;
    res_mul.mant_mul= a_mant_in * b_mant_in;
    
    //expo
    uint32_t a_expo_max   = set_expo_max(a.expo_w);
    uint32_t b_expo_max   = set_expo_max(b.expo_w);
    uint32_t res_expo_max = set_expo_max(res.expo_w);

    res_mul.expo_mul  = a.expo + static_cast<uint32_t>(fpclass1.a_expo_is_0) - (a_expo_max/2) 
                        + b.expo + static_cast<uint32_t>(fpclass1.b_expo_is_0) - (b_expo_max/2) 
                        +(res_expo_max/2); 

    res_mul.sign_mul  = a.sign ^ b.sign;
    return res_mul;
}

Sft_Inv_res cmodel_fma::fma_sft_inv(Mul_res mul_res, const FpBase& c, int a_mant_w, int b_mant_w){
    Sft_Inv_res sft_inv_res1;
    //sub
    bool sub         = mul_res.sign_mul ^ c.sign    ;
    sft_inv_res1.sub = sub                          ;
    
    //dif :: c-(a+b)
    uint32_t c_expo_max;
    uint32_t dif       ;

    c_expo_max      = set_expo_max(c.expo_w)                             ; 
    dif             = c.expo + fpclass1.c_expo_is_0 - mul_res.expo_mul   ;
    sft_inv_res1.dif= dif                                                ;
    
    //dif_sign
    uint32_t expo_sign;
    bool     dif_sign ;

    expo_sign               = get_expo_sign_bit(c.expo_w)   ;
    dif_sign                = sft_inv_res1.dif & expo_sign  ;
    sft_inv_res1.dif_sign   = dif_sign                      ;

    //expo_str and shift_num
    uint32_t shift_num   ;//
    uint32_t shift_num_uc;//shift_num_rf-dif
    uint32_t shift_num_rf;//mant_w+4
    uint32_t shift_num_ex;//the max of shift_right_num
    uint32_t expo_str    ;//to store the expo num 

    shift_num_rf = c.mant_w+4                                            ;
    shift_num_ex = a_mant_w+b_mant_w+c.mant_w+5                          ;
    shift_num_uc = shift_num_rf-sft_inv_res1.dif                         ;
    shift_num    = shift_num_uc<0            ? 0           :             
                   shift_num_uc>shift_num_ex ? shift_num_ex:             
                   shift_num_uc                                          ;
    expo_str     = sft_inv_res1.dif_sign     ? mul_res.expo_mul : c.expo ;

    sft_inv_res1.shift_num    = shift_num  ;
    sft_inv_res1.expo_str     = expo_str   ;

    //c_mant
    mp::cpp_int c_mant_in   ;
    int         c_mant_sft  ;
    mp::cpp_int c_mant_l    ;
    mp::cpp_int c_mant_inv  ;
    mp::cpp_int c_mant_s    ;
    c_mant_in       = ((1-fpclass1.c_expo_is_0)<<c.mant_w) + c.mant ;
    c_mant_sft      = a_mant_w+b_mant_w+4                           ;
    c_mant_l        = c_mant_in << c_mant_sft                       ;
    c_mant_inv      = sub ? (0-c_mant_l-1) : c_mant_l               ;

    int mant_s_width= a_mant_w+b_mant_w+c.mant_w                    ;
    //shfit with sign extension
    c_mant_s=cal_shift_r_with_sign(c_mant_inv,mant_s_width,shift_num);

    sft_inv_res1.c_mant_l     = c_mant_l      ; 
    sft_inv_res1.c_mant_inv   = c_mant_inv    ;
    sft_inv_res1.c_mant_s     = c_mant_s      ;
    sft_inv_res1.c_mant_in    = c_mant_in     ;

    return sft_inv_res1;
}

bool cmodel_fma::fma_get_st1(Sft_Inv_res sft_inv_in){
    bool st1;
    bool shift_out;
    shift_out = cal_shift_out(sft_inv_in.shift_num,sft_inv_in.c_mant_s);
    if(shift_out)//!{&bits_shift_out}
        st1=false ;
    else
        st1=true  ;
}

Sign_det_res cmodel_fma::fma_sign_det(Mul_res mul_res_in,Sft_Inv_res sft_inv_in,int expo_w_dif,bool c_sign){
    bool c_mant_bigger            ;
    bool c_bigger                 ;
    bool complement               ;
    bool sign_add                 ;

    // get c_mant_bigger and c_bigger
    mp::cpp_int mul_mant            ;
    mp::cpp_int mul_mant_vld_bits   ;
    mp::cpp_int mul_mant_lst        ; //mul_mant_lst only 1 bits 
    mp::cpp_int c_mant_ext          ;
    mp::cpp_int mul_mant_ext        ;

    mul_mant          = mul_res_in.mant_mul         ;
    mul_mant_lst      = 0                           ;
    mul_mant_vld_bits = mul_mant >> expo_w_dif      ;

    for(int i=0;i<expo_w_dif;i++){
        mul_mant_lst = mul_mant_lst & (mul_mant&0x01) ;
        mul_mant     = mul_mant >> 1                  ;
    }

    c_mant_ext        = sft_inv_in.c_mant_in<<1                ;
    mul_mant_ext      = (mul_mant_vld_bits  <<1) + mul_mant_lst;

    bool dif_is0;
    dif_is0           = sft_inv_in.dif==0               ;
    c_mant_bigger     = c_mant_ext > mul_mant_ext       ;

    c_bigger          = (dif_is0&&c_mant_bigger )|| (!sft_inv_in.dif_sign && !dif_is0)  ;
    
    //get complement and sign_add
    complement        = sft_inv_in.sub && c_bigger               ;
    sign_add          = complement ? c_sign : mul_res_in.sign_mul;

    Sign_det_res sign_det_res1;
    sign_det_res1.complement = complement;
    sign_det_res1.sign_add   = sign_add  ;

    return sign_det_res1; 
}

mp::cpp_int cmodel_fma::fma_csa_add(Csa_add_in csa_add_in,int mul_mant_w,int c_mant_w){
    //------------------
    //lowest adder:{mul_mant,sub} +{c_low_mant + !st1}
    //------------------
    mp::cpp_int mant_low_add      ;
    mp::cpp_int mant_low_add_l    ;
    mp::cpp_int add_low_av_bits   ;
    mp::cpp_int add_low_l_av_bits ;
    mp::cpp_int c_low_mant        ;
    mp::cpp_int mul_low_mant      ;

    add_low_l_av_bits = get_mant_av_bits(mul_mant_w+1)           ;
    add_low_av_bits   = get_mant_av_bits(mul_mant_w)             ;

    c_low_mant        = csa_add_in.c_mant_s_l & add_low_l_av_bits;
    mul_low_mant      = csa_add_in.mul_mant_l & add_low_l_av_bits;

    mant_low_add_l    = c_low_mant + mul_low_mant                ;
    mant_low_add      = mant_low_add_l >> 1                      ;

    //------------------
    //overlap 2 bits
    //------------------
    uint32_t c_mid_mant     ; //3+a_w+b_w~4+a_w+b_w of c_mant_s
    uint32_t add_low_ca     ; //the top 2 bits of mant_add_lower
    uint32_t mant_mid_add   ; //3 bit 1 bit represent the carry to top adder
    c_mid_mant   = static_cast<uint32_t>(csa_add_in.c_mant_s_l>>(mul_mant_w+1)&0x03);
    add_low_ca   = static_cast<uint32_t>(csa_add_in.mul_mant_l>>(mul_mant_w+1)&0x03);
    mant_mid_add = c_mid_mant + add_low_ca;

    //------------------
    //highest adder:if there is carry from lowest
    //------------------
    uint64_t    c_high_mant         ; //the highest bit of c_mant_s   2+c_mant_w
    uint64_t    mant_high_add       ; //c_high_mant+1 to be selected  
    mp::cpp_int mant_high_vld_bits  ; //width:2+c_mant_w value:all 1
    mant_high_vld_bits = get_mant_av_bits(2 + c_mant_w);
    c_high_mant        = static_cast<uint64_t>((csa_add_in.c_mant_s_l>>(3+mul_mant_w)) & mant_high_vld_bits);
    mant_high_add      = c_high_mant + 1 ;

    //------------------
    //res
    //------------------
    mp::cpp_int mant_low_res ;//a_mant_w+b_mant_w+2
    uint64_t    mant_high_res;//c_mant_w+1
    uint32_t    mant_mid_res ;//2 
    mp::cpp_int mant_add;
    //mant_low_res
    mp::cpp_int mant_low_vld_bits;
    mant_low_vld_bits = get_mant_av_bits(mul_mant_w)    ;
    mant_low_res      = mant_low_add & mant_low_vld_bits;
    
    //mant_mid_res 
    bool carry;
    carry             = mant_mid_add & 0x04;
    mant_mid_res      = mant_mid_add & 0x03;

    //mant_high_res
    if(carry)
        mant_high_res = static_cast<uint64_t>(mant_high_add & mant_high_vld_bits);
    else
        mant_high_res = c_high_mant;
    
    //
    mant_add          = (static_cast<mp::cpp_int>(mant_high_res) <<(2+mul_mant_w))
                        + (static_cast<mp::cpp_int>(mant_mid_res)<< mul_mant_w)
                        + mant_low_res;
    return mant_add;
}

mp::cpp_int cmodel_fma::fma_cas_add_cpl(Csa_add_in csa_add_in,int mul_mant_w,int c_mant_w){
    //inverse
    mp::cpp_int    mul_mant_l_inv   ;
    mp::cpp_int    c_mant_s_l_inv   ;
    
    mul_mant_l_inv = 0-csa_add_in.c_mant_s_l - 1;
    c_mant_s_l_inv = 0-csa_add_in.mul_mant_l - 1;


    //add 
    mp::cpp_int    res_vld_bits     ;//2+c_mant_w+2+2+b_mant_w+a_mant_w
    int            vld_widths       ;
    vld_widths     = 2+c_mant_w+mul_mant_w;
    res_vld_bits   = get_mant_av_bits(vld_widths);

    mp::cpp_int    add_mant_cpl_l   ;   
    mp::cpp_int    add_mant_cpl     ;
    add_mant_cpl_l = mul_mant_l_inv+c_mant_s_l_inv+2;
    
    add_mant_cpl   = (add_mant_cpl_l>>1) & res_vld_bits;
    return add_mant_cpl;
}

LOA_res cmodel_fma::fma_loa(Csa_add_in csa_add_in,int add_width){
    std::vector<int> k(add_width); 
    std::vector<int> p(add_width);
    std::vector<int> g(add_width); 
    std::vector<int> a(add_width);

    std::vector<int> p_cin(add_width+1);
    std::vector<int> f(add_width);

    BIT_CAL(p,   a_lst ^ b_lst );  
    BIT_CAL(g,   a_lst & b_lst );
    BIT_CAL(a, !(a_lst | b_lst));

    for(int i=0; i<add_width; i++){
        k[i]=p[i+1]&g[i];
    }

    for(int i=0; i<add_width; i++){
        if(i==0)
            p_cin[i] = 0;
        else
            p_cin[i] = p[i]&k[i-1] || p[i]&p_cin[i-1];
    }

    std::vector<int> cin(add_width);
    for(int i=0;i<add_width;i++){
        cin[i] = p_cin[i]||g[i];
    }
    
    //finding the first 1
    for(int i=0;i<add_width;i++){
        if(i==0)
            f[i] = !a[i];
        else
            // f[i] = !(a[i]&!(p_cin[i-1]||g[i-1]));
            f[i] = !(a[i]&!cin[i-1]);

    }
    std::vector<int> mask_vec(add_width);
    for(int i=add_width-1;i>-1; i--){
        if(i==add_width-1)
            mask_vec[i] = f[i]                 ;
        else
            mask_vec[i] = mask_vec[i+1] || f[i];
    }
    mp::cpp_int mask=0;
    for(int i=add_width-1;i>-1; i--){
        mask = (mask<<1)+mask_vec[i];
    }
    
    //finding the first 0
    std::vector<int> f_cpl(add_width);//element is 1 only when it's 0
    for(int i=0; i<add_width; i++){ 
        f_cpl[i] = (p[i]&cin[i-1])||(a[i]&&!cin[i-1])||(g[i]&&!cin[i-1]); 
    }
    
    std::vector<int> mask_vec_cpl(add_width);
    for(int i=add_width-1;i>-1; i--){
        if(i==add_width-1)
            mask_vec_cpl[i] = f_cpl[i]                     ;
        else
            mask_vec_cpl[i] = mask_vec_cpl[i+1] || f_cpl[i];
    }

    mp::cpp_int mask_cpl=0;
    for(int i=add_width-1;i>-1; i--){
        mask_cpl = (mask_cpl<<1)+mask_vec_cpl[i];
    }

    uint32_t zero_nums_0    = get_zero_nums(&mask,add_width);
    uint32_t zero_nums_1_uc = get_zero_nums(&mask_cpl,add_width);

    LOA_res loa_res_1;
    loa_res_1.zero_nums_0 = zero_nums_0;
    loa_res_1.zero_nums_1 = zero_nums_1_uc;

    return loa_res_1;
}


//cal
bool cmodel_fma::cal_shift_out(uint32_t shift_num,mp::cpp_int c_mant_s){
    bool shift_o_all_1=true;
    mp::cpp_int last_bit=1;
    for(int i=0;i<shift_num;i++){
        shift_o_all_1 = static_cast<bool>(c_mant_s&last_bit) & shift_o_all_1  ;
        c_mant_s      = c_mant_s>>1                                           ;
    }    
    return shift_o_all_1;
}

