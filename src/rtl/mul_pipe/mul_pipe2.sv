module mul_pipe2#(
    parameter int unsigned  SIGN_W = 1,
    parameter int unsigned  EXPO_W = 8,
    parameter int unsigned  MANT_W = 23,
    localparam int unsigned ZERO_D = $clog2(MANT_W + 1)
)(
    input  logic                               a_sign_reg1 ,              
    input  logic   [EXPO_W - 1 : 0]            a_expo_reg1 ,
    input  logic   [MANT_W - 1 : 0]            a_mant_reg1 ,
    input  logic                               b_sign_reg1 ,
    input  logic   [EXPO_W - 1 : 0]            b_expo_reg1 ,
    input  logic   [MANT_W - 1 : 0]            b_mant_reg1 ,   
    input  logic                               a_sub_reg1  ,              
    input  logic                               b_sub_reg1  , 
    output logic   [2*MANT_W + 1 : 0]          mant_1      ,
    output logic                               sign_1      ,
    output logic   [EXPO_W + 1 : 0]            expo_1      ,
    output logic   [$clog2(MANT_W+1)-1:0]      zero_nums_uc,
    output logic   [MANT_W : 0]                mask_short  ,
    output logic   [ZERO_D : 0]                r_shift     ,
    output logic   [ZERO_D : 0]                l_shift     
);
mul_1 #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_1(
    .a_is_sub(a_sub_reg1),
    .b_is_sub(b_sub_reg1),
    .a_mant  (a_mant_reg1),
    .b_mant  (b_mant_reg1),
    .mant_1  (mant_1)
);

mul1_res#(
    .SIGN_W(SIGN_W), 
    .EXPO_W(EXPO_W), 
    .MANT_W(MANT_W)  
)u_mul1_res(
    .a_sign_reg1     (a_sign_reg1),
    .b_sign_reg1     (b_sign_reg1),
    .a_expo_reg1     (a_expo_reg1),
    .b_expo_reg1     (b_expo_reg1),
    .a_sub_reg1      (a_sub_reg1),
    .b_sub_reg1      (b_sub_reg1),
    .a_mant_reg1     (a_mant_reg1),
    .b_mant_reg1     (b_mant_reg1),
    .sign_1          (sign_1),
    .expo_1          (expo_1),
    .zero_nums_uc    (zero_nums_uc),
    .mask_short      (mask_short),
    .r_shift         (r_shift),
    .l_shift         (l_shift)
);
endmodule