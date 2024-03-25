`include "macro_defs.sv"
module mul_pipe3#(
    parameter int unsigned  SIGN_W = 1,
    parameter int unsigned  EXPO_W = 8,
    parameter int unsigned  MANT_W = 23,
    localparam int unsigned ZERO_D = $clog2(MANT_W + 1)
)(
    `ifdef REG_3
    input  logic                               clk          ,
    `endif 
    input  logic   [MANT_W : 0]                mask_short    , 
    input  logic                               sign_1        ,  
    input  logic   [EXPO_W + 1 : 0]            expo_1        , 
    input  logic   [2*MANT_W + 1 : 0]          mant_1        , 
    input  logic   [$clog2(MANT_W + 1) -1: 0]  zero_nums_uc  , 
    input  logic   [ZERO_D : 0]                r_shift       , 
    input  logic   [ZERO_D : 0]                l_shift       , 
    input  logic   [1 : 0]                     rnd           , 
    input  logic                               a_q           ,
    input  logic                               b_q           ,
    input  logic                               a_n0          ,   
    input  logic                               b_n0          ,          
    input  logic                               a_nan         , 
    input  logic                               b_nan         , 
    input  logic                               r_nan         , 
    input  logic                               r_0nan        ,  
    input  logic                               a_sign        ,               
    input  logic   [EXPO_W - 1 : 0]            a_expo        , 
    input  logic   [MANT_W - 1 : 0]            a_mant        , 
    input  logic                               b_sign        , 
    input  logic   [EXPO_W - 1 : 0]            b_expo        , 
    input  logic   [MANT_W - 1 : 0]            b_mant        ,  
    input  logic                               inf_nan       , 
    input  logic                               a_is_nor      ,
    input  logic                               b_is_nor      ,
    input  logic                               status_nv     ,
    output logic   [4:0]                       status        , 
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
    .mask_short  (mask_short),
    .mant_1      (mant_1),
    .zero_nums_uc(zero_nums_uc),
    .zero_nums_c (zero_nums_c)
);

mul_sub_shift #(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W),
    .ZERO_D(ZERO_D)
)u_mul_sub_shift(
    .expo_1      (expo_1),
    .mant_1      (mant_1),
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
    .data_in  (mant_1),
    .shift_num(l_shift),
    .data_out (mant_2_l)
);

shifter_r#(
    .WIDTH(48)
)u_shift_r(
    .data_in  (mant_1),
    .shift_num(r_shift),
    .data_out (mant_2_r)   ,
    .r_rcd    (mant_2_r_rcd)
);

shifter_l_light#(
    .WIDTH(48)
)u_shift_r_light(
    .data_in  (mant_1),
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
    .sign_1      (sign_1),
    .mant_2      (mant_2),
    .expo_2      (expo_2),
    .bit_s_record(bit_s_record),
    .rnd         (rnd),
    .a_is_n0     (a_n0),
    .b_is_n0     (b_n0),
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
    .r_isnan    (r_nan),
    .r_is0nan   (r_0nan),
    .a_is_nan   (a_nan),
    .b_is_nan   (b_nan),
    .sign_1     (sign_1),
    .a_sign     (a_sign),
    .b_sign     (b_sign),
    .a_expo     (a_expo),
    .b_expo     (b_expo),
    .a_mant     (a_mant),
    .b_mant     (b_mant),
    .sign_nan   (sign_nan),
    .mant_4     (mant_4) 
);

mul_res_mux #(
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_res_mux(
    .is_inf_nan  (inf_nan),
    .sign_nan    (sign_nan),
    .overflow    (overflow),
    .rnd         (rnd),
    .sign_1      (sign_1),
    .mant_4      (mant_4),
    .mant_3      (mant_3),
    .expo_3      (expo_3[EXPO_W-1:0]),
    .res_sign    (res_sign),
    .res_expo    (res_expo),
    .res_mant    (res_mant)
);

mul_ecp u_mul_exp(
    .is_inf_nan (inf_nan),
    .r_is_nan   (r_nan),
    .a_is_n0    (a_n0),
    .b_is_n0    (b_n0),
    .a_is_nor   (a_is_nor),
    .b_is_nor   (b_is_nor),
    .status_nv  (status_nv),
    .overflow   (overflow),
    .underflow  (underflow),
    .inexact_rnd(inexact_rnd),
    .inexact_sft(inexact_sft),
    .status     (status)    

);

mul_pack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_pack(
    .res_sign(res_sign),
    .res_expo(res_expo),
    .res_mant(res_mant),
    .res     (res     ) 
);

`ifndef SYN
always @(mant_1 or sign_1 or expo_1) begin
    $display("@%t TIME,reg_3 triggered",$time);
    // `ifdef REG_1
    //     @(posedge clk);
    // `endif 
    // `ifdef REG_2
    //     @(posedge clk);
    //     @(negedge clk);
    // `endif 
    // `ifdef REG_3
    //     $display("-----------------------------------------");
    //     $display("reg_3 triggered");
    //     $display("-----------------------------------------");
    // `endif 
    `ifdef REG_3
        @(negedge clk);
        $display("-----------------------------------------");
        $display("reg_3 triggered");
        $display("-----------------------------------------");
    `endif 
    `echo(zero_nums_uc);
    `echo(zero_nums_c);
    `echo(mant_2_l_zn);
    `echo(l_shift);
    `echo(r_shift);
    `echo(inexact_sft);
    `echo(inexact_rnd);
    `echo(expo_3);
    `echo(mant_3);
    
    `echo(overflow);
    `echo(sign_nan);
    `echo(a_mant);
    `echo(b_mant);
    `echo(r_nan);
    `echo(mant_4);
    `echo(res_sign);
    `echo(res_expo);
    `echo(res_mant);
    `echo(status[4]);
    `echo(status[3]);
    `echo(status[2]);
    `echo(status[1]);
    `echo(status[0]);
end
`endif

endmodule