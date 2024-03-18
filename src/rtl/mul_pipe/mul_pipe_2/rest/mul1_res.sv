module mul1_res#(
    parameter  int unsigned SIGN_W =1,
    parameter  int unsigned EXPO_W = 8,
    parameter  int unsigned MANT_W = 23,
    localparam int unsigned ZERO_D = $clog2(MANT_W + 1)
)(
    input  logic                          a_sign_reg1     ,
    input  logic                          b_sign_reg1     ,
    input  logic [EXPO_W - 1 : 0]         a_expo_reg1     ,
    input  logic [EXPO_W - 1 : 0]         b_expo_reg1     ,
    input  logic                          a_sub_reg1      ,
    input  logic                          b_sub_reg1      ,
    input  logic [MANT_W - 1 : 0]         a_mant_reg1     ,
    input  logic [MANT_W - 1 : 0]         b_mant_reg1     ,
    output logic                          sign_1          ,
    output logic [EXPO_W + 1 : 0]         expo_1          ,
    output logic [$clog2(MANT_W+1)-1:0]   zero_nums_uc    ,
    output logic [MANT_W : 0]             mask_short      ,
    output logic [ZERO_D : 0]             r_shift         ,
    output logic [ZERO_D : 0]             l_shift       
);
//------internal signals
logic   [MANT_W : 0]        mant_input  ;

mul_expo_1#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_expo_1(
    .a_sign  (a_sign_reg1  ),
    .b_sign  (b_sign_reg1  ),
    .a_is_sub(a_sub_reg1   ),
    .b_is_sub(b_sub_reg1   ),
    .a_expo  (a_expo_reg1  ),
    .b_expo  (b_expo_reg1  ),
    .sign_1  (sign_1       ),
    .expo_1  (expo_1       )
);

mul_short_judge#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_short_judge(
    .a_is_sub  (a_sub_reg1     ),
    .b_is_sub  (b_sub_reg1     ),
    .a_mant    (a_mant_reg1    ),       
    .b_mant    (b_mant_reg1    ),
    .mant_input(mant_input     )  
);

mul_lod #(
    .INPUT_WIDTH(MANT_W+1)
)u_mul_lod(
    .in_detect(mant_input),
    .zero_nums(zero_nums_uc)
);

mul_mask #(
    .INPUT_WIDTH(MANT_W+1) 
)u_mul_mask(
    .input_unmask(mant_input),
    .output_mask(mask_short) 
);

mul_shift_num#(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W),
    .ZERO_D(ZERO_D)
)
u_mul_shift_num(
    .expo_1 (expo_1 )   ,
    .r_shift(r_shift)   ,
    .l_shift(l_shift)   
);

endmodule