module mul_pipe3#(
    parameter int unsigned  SIGN_W = 1,
    parameter int unsigned  EXPO_W = 8,
    parameter int unsigned  MANT_W = 23,
    localparam int unsigned ZERO_D = $clog2(MANT_W + 1)

)(
    input  logic   [MANT_W : 0]                mask_short_reg2    , 
    input  logic                               sign_1_reg2        ,  
    input  logic   [EXPO_W + 1 : 0]            expo_1_reg2        , 
    input  logic   [2*MANT_W + 1 : 0]          mant_1_reg2        , 
    input  logic   [$clog2(MANT_W + 1) -1: 0]  z_nums_uc_reg2     , 
    input  logic   [ZERO_D : 0]                r_shift_reg2       , 
    input  logic   [ZERO_D : 0]                l_shift_reg2       , 
    input  logic   [1 : 0]                     rnd_reg2           , 
    input  logic                               a_q_reg2           ,
    input  logic                               b_q_reg2           ,
    input  logic                               a_n0_reg2          ,   
    input  logic                               b_n0_reg2          ,          
    input  logic                               a_nan_reg2         , 
    input  logic                               b_nan_reg2         , 
    input  logic                               r_nan_reg2         , 
    input  logic                               r_0nan_reg2        ,  
    input  logic                               a_sign_reg2        ,               
    input  logic   [EXPO_W - 1 : 0]            a_expo_reg2        , 
    input  logic   [MANT_W - 1 : 0]            a_mant_reg2        , 
    input  logic                               b_sign_reg2        , 
    input  logic   [EXPO_W - 1 : 0]            b_expo_reg2        , 
    input  logic   [MANT_W - 1 : 0]            b_mant_reg2        ,  
    input  logic                               inf_nan_reg2       ,    
    output logic   [4:0]                       status             , 
    output logic   [EXPO_W + MANT_W : 0 ]      res 
);

//---------internal signals
logic   [ZERO_D -1: 0]              zero_nums_c ;
logic   [2*MANT_W + 1: 0]           mant_2_l    ;
logic   [2*MANT_W + 1: 0]           mant_2_l_zn ;
logic   [2*MANT_W + 1: 0]           mant_2_r    ;
logic                               mant_2_r_rcd;
logic                               underflow   ;
logic                               inexact_sft ;
logic                               inexact_rnd ;
logic                               bit_s_record;
logic                               r_rcd       ;
logic   [EXPO_W + 1  : 0]           expo_2      ;
logic   [2*MANT_W + 1: 0]           mant_2      ;
logic   [EXPO_W + 1 : 0]            expo_3      ;
logic   [MANT_W - 1 : 0]            mant_3      ;
logic                               overflow    ;
logic   [MANT_W - 1 : 0]            mant_4      ;
logic                               sign_nan    ;
logic                               res_sign    ;
logic   [EXPO_W - 1 : 0]            res_expo    ;
logic   [MANT_W - 1 : 0]            res_mant    ;

mul_rev #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_rev(      
    .mask_short  (mask_short_reg2),
    .mant_1      (mant_1_reg2),
    .zero_nums_uc(z_nums_uc_reg2),
    .zero_nums_c (zero_nums_c)
);

mul_sub_shift #(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W),
    .ZERO_D(ZERO_D)
)u_mul_sub_shift(
    .expo_1      (expo_1_reg2),
    .mant_1      (mant_1_reg2),
    .zero_nums_c (zero_nums_c),
    .mant_2_l    (mant_2_l),
    .mant_2_r    (mant_2_r),
    .mant_2_r_rcd(mant_2_r_rcd),
    .mant_2_l_zn (mant_2_l_zn),
    .expo_2      (expo_2),
    .mant_2      (mant_2),
    .underflow   (underflow),
    .inexact_sft (inexact_sft),
    .bit_s_record(bit_s_record)
);

shifter_l#(
    .WIDTH(48)
)u_shift_l(
    .data_in  (mant_1_reg2),
    .shift_num(l_shift_reg2),
    .data_out (mant_2_l)
);

shifter_r#(
    .WIDTH(48)
)u_shift_r(
    .data_in  (mant_1_reg2),
    .shift_num(r_shift_reg2),
    .data_out (mant_2_r)   ,
    .r_rcd    (r_rcd)
);

shifter_l_light#(
    .WIDTH(48)
)u_shift_r_light(
    .data_in  (mant_1_reg2),
    .shift_num(zero_nums_c),
    .data_out (mant_2_l_zn)
);

//-------------------------------
//rounding
//-------------------------------
mul_rnd #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_rnd(
    .sign_1      (sign_1_reg2),
    .mant_2      (mant_2),
    .expo_2      (expo_2),
    .bit_s_record(bit_s_record),
    .rnd         (rnd_reg2),
    .a_is_n0     (a_n0_reg2),
    .b_is_n0     (b_n0_reg2),
    .inexact_rnd (inexact_rnd),
    .expo_3      (expo_3),
    .mant_3      (mant_3)
);

//-------------------------------
//inf and nan mux
//-------------------------------
mul_ovf_judge #(
    .EXPO_W(EXPO_W)
)u_mul_ovf_judge(
    .expo_2(expo_2),
    .expo_3(expo_3),
    .overflow(overflow)  
);

 mul_nan_judge #(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
 )u_mul_nan_judge(
    .r_isnan    (r_nan_reg2),
    .r_is0nan   (r_0nan_reg2),
    .a_is_nan   (a_nan_reg2),
    .b_is_nan   (b_nan_reg2),
    .sign_1     (sign_1_reg2),
    .a_sign     (a_sign_reg2),
    .b_sign     (b_sign_reg2),
    .a_expo     (a_expo_reg2),
    .b_expo     (b_expo_reg2),
    .a_mant     (a_mant_reg2),
    .b_mant     (b_mant_reg2),
    .sign_nan   (sign_nan),
    .mant_4     (mant_4) 
);

mul_res_mux #(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_res_mux(
    .is_inf_nan  (inf_nan_reg2),
    .sign_nan    (sign_nan),
    .overflow    (overflow),
    .rnd         (rnd_reg2),
    .sign_1      (sign_1_reg2),
    .mant_4      (mant_4),
    .mant_3      (mant_3),
    .expo_3      (expo_3[EXPO_W-1:0]),
    .res_sign    (res_sign),
    .res_expo    (res_expo),
    .res_mant    (res_mant)
);

mul_ecp#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_exp(
    .a_is_q     (a_q_reg2), 
    .b_is_q     (b_q_reg2),
    .a_is_nan   (a_nan_reg2),
    .b_is_nan   (b_nan_reg2),
    .r_is_0nan  (r_0nan_reg2), 
    .overflow   (overflow),
    .underflow  (underflow),
    .inexact_rnd(inexact_rnd),
    .inexact_sft(inexact_sft),
    .status     (status)
);

always_comb begin:mul_pack
    res = {res_sign,res_expo,res_mant};
end

endmodule