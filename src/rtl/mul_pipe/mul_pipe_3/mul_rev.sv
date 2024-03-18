module mul_rev #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(      
    input  logic [MANT_W : 0]                mask_short  ,
    input  logic [2*MANT_W+1 : 0]            mant_1      ,
    input  logic [$clog2(MANT_W + 1) -1: 0]  zero_nums_uc,
    output logic [$clog2(MANT_W + 1) -1: 0]  zero_nums_c 
);
    logic                               revise      ;
    logic   [2*MANT_W + 1 : 0]          mask_long   ;
    
assign mask_long  = {mask_short,{(MANT_W+1){1'b0}}}           ;
assign revise     = ((mant_1 & mask_long)) ? 1'b1 : 1'b0      ;
assign zero_nums_c= revise ? zero_nums_uc - 1 : zero_nums_uc  ;

endmodule