module mul_para #(
    parameter  int unsigned SIGN_W = 1,
    parameter  int unsigned EXPO_W = 8,
    parameter  int unsigned MANT_W = 23,
    localparam int unsigned ZERO_D = $clog2(MANT_W + 1)
) (
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     a   ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     b   ,
    input  logic [1 : 0]                               rnd ,
    output logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     res  
);

//-----------------------------------//
//internal signals
//-----------------------------------//
//-------------nan flag
logic   a_is_sub     ;
logic   b_is_sub     ;
logic   a_is_nan    ;
logic   b_is_nan    ;
// logic   a_is_inf    ;
logic   b_is_inf    ;
logic   a_is_n0      ;
logic   b_is_n0      ;

//-------------unpack signals
logic                               a_sign      ;              
logic   [EXPO_W - 1 : 0]            a_expo      ;
logic   [MANT_W - 1 : 0]            a_mant      ;
logic                               b_sign      ;
logic   [EXPO_W - 1 : 0]            b_expo      ;
logic   [MANT_W - 1 : 0]            b_mant      ;

//-------------pack signals
logic                               res_sign    ;
logic   [EXPO_W - 1 : 0]            res_expo    ;
logic   [MANT_W - 1 : 0]            res_mant    ;


//-------------temp signals
logic                               sign_1      ;
logic   [EXPO_W + 1 : 0]            expo_1      ;
logic   [2*MANT_W + 1 : 0]          mant_1      ;

//-------------lod signals
logic   [MANT_W   : 0]              mant_input  ;
logic   [ZERO_D -1: 0]              zero_nums_c ;
logic   [$clog2(MANT_W + 1) -1: 0]  zero_nums_uc;
logic   [MANT_W : 0]                mask_short  ;


//-------------expo adjust and mant shift signals
logic   [EXPO_W + 1  : 0]           expo_2      ;
logic   [2*MANT_W + 1: 0]           mant_2      ;
logic   [2*MANT_W + 1: 0]           mant_2_l    ;
logic   [2*MANT_W + 1: 0]           mant_2_l_zn ;

logic   [2*MANT_W + 1: 0]           mant_2_r    ;
// logic   [EXPO_W + 1  : 0]           r_shift_unc ;
// logic   [EXPO_W + 1  : 0]           l_shift_unc ;
logic   [ZERO_D : 0]                r_shift     ;
logic   [ZERO_D : 0]                l_shift     ;
//-------------round signals
logic                               bit_s_record;
logic                               g           ;
logic                               r           ;
logic                               s           ;
logic                               carry       ;
logic   [MANT_W + 1 : 0]            mant_inr_1  ;
logic   [EXPO_W + 1 : 0]            expo_inr_1  ;
logic   [MANT_W + 1 : 0]            mant_rnd    ;
logic   [EXPO_W + 1 : 0]            expo_rnd    ;
logic   [EXPO_W + 1 : 0]            expo_3      ;
logic   [MANT_W - 1 : 0]            mant_3      ;
//-------------inf and nan signals 
logic                               overflow    ;
logic                               r_isnan     ;
logic                               is_inf_nan  ;
logic                               r_is0nan    ;
logic                               sign_nan    ;
logic   [MANT_W - 1 : 0]            mant_4      ;
//-------------debug signals
logic   [EXPO_W + 1 : 0]            expo_d_1    ;
logic   [EXPO_W + 1 : 0]            expo_d_2    ;

//-----------------------------------
//unpack 
//-----------------------------------
mul_unpack#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_unpack(
    .a(a)           ,
    .b(b)           ,
    .a_sign(a_sign) ,
    .b_sign(b_sign) ,
    .a_expo(a_expo) ,
    .b_expo(b_expo) ,
    .a_mant(a_mant) ,
    .b_mant(b_mant) 
);
//-----------------------------------
//fix cal
//-----------------------------------
mul_fixcal #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_fixcal(
    // .a_sign(a_sign)         ,
    // .b_sign(b_sign)         ,
    .a_expo(a_expo)         ,
    .b_expo(b_expo)         ,
    .a_mant(a_mant)         ,
    .b_mant(b_mant)         ,
    .a_is_sub(a_is_sub)     ,
    .b_is_sub(b_is_sub)     ,
    .a_is_n0(a_is_n0)       , 
    .b_is_n0(b_is_n0)       ,
    .a_is_nan(a_is_nan)     ,
    .b_is_nan(b_is_nan)     ,
    // .a_is_inf(a_is_inf)     ,
    // .b_is_inf(b_is_inf)     ,
    .r_isnan(r_isnan)       ,
    .is_inf_nan(is_inf_nan) ,
    .r_is0nan(r_is0nan)    
);
//-----------------------------------
//fix cal
//-----------------------------------
mul_1 #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_1(
    .a_sign(a_sign)     ,
    .b_sign(b_sign)     ,
    .a_is_sub(a_is_sub) ,
    .b_is_sub(b_is_sub) ,
    .a_expo(a_expo)     ,
    .b_expo(b_expo)     ,
    .a_mant(a_mant)     ,
    .b_mant(b_mant)     ,
    .sign_1(sign_1)     ,
    .expo_1(expo_1)     ,
    .mant_1(mant_1)  
);
//-----------------------------------
//loz
//-----------------------------------
mul_short_judge #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_short_judge(
    .a_is_sub(a_is_sub)       ,
    .b_is_sub(b_is_sub)       ,
    .a_mant(a_mant)           ,       
    .b_mant(b_mant)           ,
    .mant_input(mant_input)
);

mul_lod #(
    .INPUT_WIDTH(MANT_W+1))
u_lod(
    .in_detect(mant_input),
    .zero_nums(zero_nums_uc)
);

mul_mask #(
    .INPUT_WIDTH(MANT_W+1))
u_mask(
    .input_unmask(mant_input),
    .output_mask(mask_short)
);

mul_rev #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_rev(
    .mask_short(mask_short)    ,
    .mant_1(mant_1)            ,
    .zero_nums_uc(zero_nums_uc),
    .zero_nums_c(zero_nums_c) 
);
//----------------------------------------------------------
//expo adjust and mant shift
//----------------------------------------------------------
mul_shift_num#(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W),
    .ZERO_D(ZERO_D)
)u_mul_shift_num(
    .expo_1  (expo_1  ),
    .r_shift (r_shift ),
    .l_shift (l_shift )
);
// logic zero_num_sel;
mul_sub_shift #(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W),
    .ZERO_D(ZERO_D)
)u_mul_sub_shift(
    .expo_1      (expo_1      ),
    .mant_1      (mant_1      ),
    .zero_nums_c (zero_nums_c ),
    .mant_2_l    (mant_2_l    ),
    .mant_2_r    (mant_2_r    ),
    .mant_2_l_zn (mant_2_l_zn ),
    .expo_2      (expo_2      ),
    .mant_2      (mant_2      ),
    .bit_s_record(bit_s_record)
);

shifter_r#(
    .WIDTH(48)
)u_shifter_r(
    .data_in(mant_1)     ,
    .shift_num(r_shift)   ,
    .data_out(mant_2_r)
);

shifter_l#(
    .WIDTH(48)
)u_shifter_l(
    .data_in(mant_1)     ,
    .shift_num(l_shift)   ,
    .data_out(mant_2_l)
);

shifter_r_light#(
    .WIDTH(48)
)u_shifter_l_zn(
    .data_in(mant_1)     ,
    .shift_num(zero_nums_c)   ,
    .data_out(mant_2_l_zn)
);

//---------------------------------------------------
//rounding
//---------------------------------------------------
mul_rnd #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_rnd(
    .sign_1(sign_1)             ,
    .mant_2(mant_2)             ,
    .expo_2(expo_2)             ,
    .bit_s_record(bit_s_record) ,
    .rnd(rnd)                   ,
    .a_is_n0(a_is_n0)           ,
    .b_is_n0(b_is_n0)           ,
    .expo_3(expo_3)             ,
    .mant_3(mant_3)                  
);
//---------------------------------------------------
//inf and nan mux
//---------------------------------------------------

mul_ovf_judge #(
    .EXPO_W(EXPO_W)
)u_mul_ovf_judge(
    .expo_2(expo_2),
    .expo_3(expo_3),
    .overflow(overflow)
);
mul_nan_judge#(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_nan_judge(
    .r_isnan    (r_isnan    ),
    .r_is0nan   (r_is0nan   ),
    .a_is_nan   (a_is_nan   ),
    .b_is_nan   (b_is_nan   ),
    .sign_1     (sign_1     ),
    .a_sign     (a_sign     ),
    .b_sign     (b_sign     ),
    .a_expo     (a_expo     ),
    .b_expo     (b_expo     ),
    .a_mant     (a_mant     ),
    .b_mant     (b_mant     ),
    .sign_nan   (sign_nan   ),
    .mant_4     (mant_4     )
);
mul_res_mux#(
    .EXPO_W(EXPO_W) ,
    .MANT_W(MANT_W)
)u_mul_res_mux(
    .is_inf_nan  (is_inf_nan        ),
    .sign_nan    (sign_nan          ),
    .overflow    (overflow          ),
    .rnd         (rnd               ),
    .sign_1      (sign_1            ),
    .mant_4      (mant_4            ),
    .mant_3      (mant_3            ),
    .expo_3      (expo_3[EXPO_W-1:0]),
    .res_sign    (res_sign          ),
    .res_expo    (res_expo          ),
    .res_mant    (res_mant          )
);
always_comb begin:mul_pack
    res  =  {res_sign,res_expo,res_mant};
end
endmodule