`include "macro_defs.sv"
module mul_pipe2#(
    parameter int unsigned  SIGN_W = 1,
    parameter int unsigned  EXPO_W = 8,
    parameter int unsigned  MANT_W = 23,
    localparam int unsigned ZERO_D = $clog2(MANT_W + 1)
)(
    `ifdef REG_2
    input  logic                               clk          ,
    `endif 
    input  logic                               a_sign       ,              
    input  logic   [EXPO_W - 1 : 0]            a_expo       ,
    input  logic   [MANT_W - 1 : 0]            a_mant       ,
    input  logic                               b_sign       ,
    input  logic   [EXPO_W - 1 : 0]            b_expo       ,
    input  logic   [MANT_W - 1 : 0]            b_mant       ,   
    input  logic                               a_sub        ,              
    input  logic                               b_sub        , 

    input  logic                               a_nan_in     ,   
    input  logic                               b_nan_in     ,   
    input  logic                               a_n0_in      ,   
    input  logic                               b_n0_in      ,     
    input  logic                               r_nan_in     ,   
    input  logic                               inf_nan_in   ,   
    input  logic                               r_0nan_in    ,   
    input  logic   [1 : 0]                     rnd_in       , 
    input  logic                               a_is_nor_in  ,    
    input  logic                               b_is_nor_in  ,    
    input  logic                               status_nv_in ,   

    output logic                               a_nan_out    ,
    output logic                               b_nan_out    ,
    output logic                               a_n0_out     ,
    output logic                               b_n0_out     ,
    output logic                               a_q_out      ,
    output logic                               b_q_out      ,
    output logic                               r_nan_out    ,
    output logic                               inf_nan_out  ,
    output logic                               r_0nan_out   ,
    output logic   [1 : 0]                     rnd_out      ,

    output logic                               a_sign_out   ,              
    output logic   [EXPO_W - 1 : 0]            a_expo_out   ,
    output logic   [MANT_W - 1 : 0]            a_mant_out   ,
    output logic                               b_sign_out   ,
    output logic   [EXPO_W - 1 : 0]            b_expo_out   ,
    output logic   [MANT_W - 1 : 0]            b_mant_out   , 

    output logic   [2*MANT_W + 1 : 0]          mant_1       ,
    output logic                               sign_1       ,
    output logic   [EXPO_W + 1 : 0]            expo_1       ,
    output logic   [$clog2(MANT_W+1)-1:0]      zero_nums_uc ,
    output logic   [MANT_W : 0]                mask_short   ,
    output logic   [ZERO_D : 0]                r_shift      ,
    output logic   [ZERO_D : 0]                l_shift      ,
    output logic                               a_is_nor_out ,    
    output logic                               b_is_nor_out ,    
    output logic                               status_nv_out  
);

//---------pipe reg signals
logic                               sign_1_reg    ;
logic   [EXPO_W + 1 : 0]            expo_1_reg    ;
logic   [2*MANT_W + 1 : 0]          mant_1_reg    ;
logic   [$clog2(MANT_W + 1) -1: 0]  z_nums_uc_reg ;
logic   [MANT_W : 0]                mask_short_reg;
logic   [ZERO_D : 0]                r_shift_reg   ;
logic   [ZERO_D : 0]                l_shift_reg   ;

logic                               a_nan_reg     ;
logic                               b_nan_reg     ;
logic                               a_n0_reg      ;
logic                               b_n0_reg      ;
logic                               a_q_reg       ;
logic                               b_q_reg       ;
logic                               r_nan_reg     ;
logic                               inf_nan_reg   ;
logic                               r_0nan_reg    ;
logic   [1 : 0]                     rnd_reg       ;

logic                               a_sign_reg    ;              
logic   [EXPO_W - 1 : 0]            a_expo_reg    ;
logic   [MANT_W - 1 : 0]            a_mant_reg    ;
logic                               b_sign_reg    ;
logic   [EXPO_W - 1 : 0]            b_expo_reg    ;
logic   [MANT_W - 1 : 0]            b_mant_reg    ;  
logic                               a_is_nor_reg  ;
logic                               b_is_nor_reg  ;
logic                               status_nv_reg ;

//---------pipe comb signals
logic                               sign_1_comb    ;
logic   [EXPO_W + 1 : 0]            expo_1_comb    ;
logic   [2*MANT_W + 1 : 0]          mant_1_comb    ;
logic   [$clog2(MANT_W + 1) -1: 0]  z_nums_uc_comb ;
logic   [MANT_W : 0]                mask_short_comb;
logic   [ZERO_D : 0]                r_shift_comb   ;
logic   [ZERO_D : 0]                l_shift_comb   ;


mul_1 #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_1(
    .a_is_sub(a_sub),
    .b_is_sub(b_sub),
    .a_mant  (a_mant),
    .b_mant  (b_mant),
    .mant_1  (mant_1_comb)
);

mul1_res#(
    .SIGN_W(SIGN_W), 
    .EXPO_W(EXPO_W), 
    .MANT_W(MANT_W)  
)u_mul1_res(
    .a_sign          (a_sign),
    .b_sign          (b_sign),
    .a_expo          (a_expo),
    .b_expo          (b_expo),
    .a_sub           (a_sub),
    .b_sub           (b_sub),
    .a_mant          (a_mant),
    .b_mant          (b_mant),
    .sign_1          (sign_1_comb),
    .expo_1          (expo_1_comb),
    .zero_nums_uc    (z_nums_uc_comb),
    .mask_short      (mask_short_comb),
    .r_shift         (r_shift_comb),
    .l_shift         (l_shift_comb)
);

`ifdef REG_2
always_ff @(posedge clk) begin
    sign_1_reg    <= sign_1_comb        ;
    expo_1_reg    <= expo_1_comb        ;
    mant_1_reg    <= mant_1_comb        ;
    z_nums_uc_reg <= z_nums_uc_comb     ;
    mask_short_reg<= mask_short_comb    ;
    r_shift_reg   <= r_shift_comb       ;
    l_shift_reg   <= l_shift_comb       ;

    a_nan_reg     <= a_nan_in     ;
    b_nan_reg     <= b_nan_in     ;
    a_n0_reg      <= a_n0_in      ;
    b_n0_reg      <= b_n0_in      ;
    r_nan_reg     <= r_0nan_in    ;
    inf_nan_reg   <= inf_nan_in   ;
    r_0nan_reg    <= r_0nan_in    ;
    rnd_reg       <= rnd_in       ;

    a_sign_reg    <= a_sign       ;
    a_expo_reg    <= a_expo       ;
    a_mant_reg    <= a_mant       ;
    b_sign_reg    <= b_sign       ;
    b_expo_reg    <= b_expo       ;
    b_mant_reg    <= b_mant       ;

    a_is_nor_reg  <= a_is_nor_in  ;
    b_is_nor_reg  <= b_is_nor_in  ;
    status_nv_reg <= status_nv_in ;
end
always_comb begin
    a_nan_out    = a_nan_reg      ; 
    b_nan_out    = b_nan_reg      ;
    a_n0_out     = a_n0_reg       ;
    b_n0_out     = b_n0_reg       ;
    a_q_out      = a_q_reg        ;
    b_q_out      = b_q_reg        ;
    r_nan_out    = r_nan_reg      ; 
    inf_nan_out  = inf_nan_reg    ;
    r_0nan_out   = r_0nan_reg     ; 
    rnd_out      = rnd_reg        ;

    a_sign_out   = a_sign_reg     ;              
    a_expo_out   = a_expo_reg     ;
    a_mant_out   = a_mant_reg     ;
    b_sign_out   = b_sign_reg     ;
    b_expo_out   = b_expo_reg     ;
    b_mant_out   = b_mant_reg     ;

    mant_1       = mant_1_reg     ;
    sign_1       = sign_1_reg     ;
    expo_1       = expo_1_reg     ;
    zero_nums_uc = z_nums_uc_reg  ;
    mask_short   = mask_short_reg ;
    r_shift      = r_shift_reg    ;
    l_shift      = l_shift_reg    ;

    a_is_nor_out = a_is_nor_reg   ;
    b_is_nor_out = b_is_nor_reg   ;
    status_nv_out= status_nv_reg  ;
end
`else
always_comb begin
    a_nan_out    = a_nan_in       ; 
    b_nan_out    = b_nan_in       ;
    a_n0_out     = a_n0_in        ;
    b_n0_out     = b_n0_in        ;
    r_nan_out    = r_nan_in       ; 
    inf_nan_out  = inf_nan_in     ;
    r_0nan_out   = r_0nan_in      ; 
    rnd_out      = rnd_in         ;
 
    a_sign_out   = a_sign         ;              
    a_expo_out   = a_expo         ;
    a_mant_out   = a_mant         ;
    b_sign_out   = b_sign         ;
    b_expo_out   = b_expo         ;
    b_mant_out   = b_mant         ;

    mant_1       = mant_1_comb    ;
    sign_1       = sign_1_comb    ;
    expo_1       = expo_1_comb    ;
    zero_nums_uc = z_nums_uc_comb ;
    mask_short   = mask_short_comb;
    r_shift      = r_shift_comb   ;
    l_shift      = l_shift_comb   ;

    a_is_nor_out = a_is_nor_in    ;
    b_is_nor_out = b_is_nor_in    ;
    status_nv_out= status_nv_in   ;
end
`endif

`ifndef SYN
always @(a_sign or a_expo or a_mant or b_sign or b_expo or b_mant) begin
    $display("@%t TIME,reg_2 triggered",$time);
    // `ifdef REG_1
    //     @(posedge clk);
    // `endif 
    // `ifdef REG_2
    //     @(posedge clk);
    //     @(negedge clk);
    //     $display("-----------------------------------------");
    //     $display("reg_2 trigged");
    //     $display("-----------------------------------------");
    // `endif 
    `ifdef REG_2
        @(posedge clk);
        @(negedge clk);
        $display("-----------------------------------------");
        $display("reg_2 trigged");
        $display("-----------------------------------------");
    `endif 
    `echo(sign_1);
    `echo(expo_1);
    `echo(mant_1);
    `echo(r_shift);
    `echo(l_shift);
    `echo(z_nums_uc_comb);
    `echo(mask_short_comb);
end
`endif

endmodule