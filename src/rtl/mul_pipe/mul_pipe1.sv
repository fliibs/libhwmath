`include "macro_defs.sv"
module mul_pipe1#(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(
    input  logic [SIGN_W + EXPO_W + MANT_W - 1 : 0] a         ,
    input  logic [SIGN_W + EXPO_W + MANT_W - 1 : 0] b         ,
    input  logic [1 : 0 ]                           rnd       ,
    `ifdef REG_1
    input  logic                                    clk       ,
    `endif
    output logic                                    a_sign    ,
    output logic                                    b_sign    ,
    output logic [EXPO_W - 1 : 0]                   a_expo    ,
    output logic [EXPO_W - 1 : 0]                   b_expo    ,
    output logic [MANT_W - 1 : 0]                   a_mant    ,
    output logic [MANT_W - 1 : 0]                   b_mant    ,
    output logic                                    a_is_sub  ,
    output logic                                    b_is_sub  ,
    output logic                                    a_is_n0   ,
    output logic                                    b_is_n0   ,
    output logic                                    a_is_nan  ,
    output logic                                    b_is_nan  ,
    output logic                                    r_isnan   ,
    output logic                                    is_inf_nan,
    output logic                                    r_is0nan  ,
    output logic [1 : 0]                            rnd_out   ,
    output logic                                    a_is_nor  ,
    output logic                                    b_is_nor  ,
    output logic                                    status_nv 
);

//---------pipe1 reg signals
logic                               a_sign_reg     ;              
logic   [EXPO_W - 1 : 0]            a_expo_reg     ;
logic   [MANT_W - 1 : 0]            a_mant_reg     ;
logic                               b_sign_reg     ;
logic   [EXPO_W - 1 : 0]            b_expo_reg     ;
logic   [MANT_W - 1 : 0]            b_mant_reg     ;   
logic                               a_sub_reg      ;              
logic                               b_sub_reg      ;              
logic                               a_nan_reg      ;
logic                               b_nan_reg      ;
logic                               a_q_reg        ;
logic                               b_q_reg        ;
logic                               a_n0_reg       ;
logic                               b_n0_reg       ;
logic                               r_nan_reg      ;
logic                               inf_nan_reg    ;
logic                               r_0nan_reg     ;
logic   [1 : 0]                     rnd_reg        ;
logic                               a_is_nor_reg   ;
logic                               b_is_nor_reg   ;
logic                               status_nv_reg  ;

//---------pipe1 comb signals
logic                               a_sign_comb    ;              
logic   [EXPO_W - 1 : 0]            a_expo_comb    ;
logic   [MANT_W - 1 : 0]            a_mant_comb    ;
logic                               b_sign_comb    ;
logic   [EXPO_W - 1 : 0]            b_expo_comb    ;
logic   [MANT_W - 1 : 0]            b_mant_comb    ;   
logic                               a_sub_comb     ;              
logic                               b_sub_comb     ;              
logic                               a_nan_comb     ;
logic                               b_nan_comb     ;
logic                               a_q_comb       ;
logic                               b_q_comb       ;
logic                               a_n0_comb      ;
logic                               b_n0_comb      ;
logic                               r_nan_comb     ;
logic                               inf_nan_comb   ;
logic                               r_0nan_comb    ;
logic                               a_is_nor_comb  ;
logic                               b_is_nor_comb  ;
logic                               status_nv_comb ;
//-----------------------------
//unpack
//-----------------------------
mul_unpack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_unpack(
    .a       (a),
    .b       (b),
    .a_sign  (a_sign_comb),
    .b_sign  (b_sign_comb),
    .a_expo  (a_expo_comb),
    .b_expo  (b_expo_comb),
    .a_mant  (a_mant_comb),
    .b_mant  (b_mant_comb)
);
//-----------------------------
//fix cal
//-----------------------------
mul_fixcal #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_fixcal(
    .a_expo    (a_expo_comb    ),
    .b_expo    (b_expo_comb    ),
    .a_mant    (a_mant_comb    ),
    .b_mant    (b_mant_comb    ),
    .a_is_sub  (a_is_sub_comb  ),
    .b_is_sub  (b_is_sub_comb  ),
    .a_is_n0   (a_is_n0_comb   ),
    .b_is_n0   (b_is_n0_comb   ),
    .a_is_nan  (a_is_nan_comb  ),
    .b_is_nan  (b_is_nan_comb  ),
    .r_isnan   (r_isnan_comb   ),
    .is_inf_nan(is_inf_nan_comb),
    .r_is0nan  (r_is0nan_comb  ),
    .a_is_nor  (a_is_nor_comb  ),
    .b_is_nor  (b_is_nor_comb  ),
    .status_nv (status_nv_comb )
);

`ifdef REG_1
always_ff @( posedge clk) begin 
    a_sign_reg    <= a_sign_comb       ;
    a_expo_reg    <= a_expo_comb       ;
    a_mant_reg    <= a_mant_comb       ;
    b_sign_reg    <= b_sign_comb       ;
    b_expo_reg    <= b_expo_comb       ;
    b_mant_reg    <= b_mant_comb       ;
    a_sub_reg     <= a_is_sub_comb     ;
    b_sub_reg     <= b_is_sub_comb     ;
    a_nan_reg     <= a_is_nan_comb     ;
    b_nan_reg     <= b_is_nan_comb     ; 
    a_n0_reg      <= a_is_n0_comb      ; 
    b_n0_reg      <= b_is_n0_comb      ;
    r_nan_reg     <= r_isnan_comb      ;
    inf_nan_reg   <= is_inf_nan_comb   ;
    r_0nan_reg    <= r_is0nan_comb     ;
    rnd_reg       <= rnd               ;
    a_is_nor_reg  <= a_is_nor_comb     ;
    b_is_nor_reg  <= b_is_nor_comb     ;
    status_nv_reg <= status_nv_comb    ;
end
always_comb begin
    a_sign    = a_sign_reg      ;
    b_sign    = b_sign_reg      ;
    a_expo    = a_expo_reg      ;
    b_expo    = b_expo_reg      ;
    a_mant    = a_mant_reg      ;
    b_mant    = b_mant_reg      ;
    a_is_sub  = a_sub_reg       ;
    b_is_sub  = b_sub_reg       ;
    a_is_n0   = a_n0_reg        ;
    b_is_n0   = b_n0_reg        ;
    a_is_nan  = a_nan_reg       ;
    b_is_nan  = b_nan_reg       ;
    r_isnan   = r_nan_reg       ;
    is_inf_nan= inf_nan_reg     ;
    r_is0nan  = r_0nan_reg      ;
    rnd_out   = rnd_reg         ;
    a_is_nor  = a_is_nor_reg    ;
    b_is_nor  = b_is_nor_reg    ;
    status_nv = status_nv_reg   ;
    end
`else
always_comb begin
    a_sign    = a_sign_comb     ;
    b_sign    = b_sign_comb     ;
    a_expo    = a_expo_comb     ;
    b_expo    = b_expo_comb     ;
    a_mant    = a_mant_comb     ;
    b_mant    = b_mant_comb     ;
    a_is_sub  = a_sub_comb      ;
    b_is_sub  = b_sub_comb      ;
    a_is_n0   = a_n0_comb       ;
    b_is_n0   = b_n0_comb       ;
    a_is_nan  = a_nan_comb      ;
    b_is_nan  = b_nan_comb      ;
    r_isnan   = r_nan_comb      ;
    is_inf_nan= inf_nan_comb    ;
    r_is0nan  = r_0nan_comb     ;
    rnd_out   = rnd             ;
    a_is_nor  = a_is_nor_comb   ;
    b_is_nor  = b_is_nor_comb   ;
    status_nv = status_nv_comb  ;
end
`endif

`ifndef SYN
always @ (a or b or rnd) begin
    $display("@%t TIME,reg_1 triggered",$time);
    // `ifdef REG_1
    //     @(posedge clk);
    //     @(negedge clk);
    //     $display("-----------------------------------------");
    //     $display("reg_1 trigged");
    //     $display("-----------------------------------------");
    // `endif
    `ifdef REG_1
        @(posedge clk);
        @(negedge clk);
        $display("-----------------------------------------");
        $display("reg_1 trigged");
        $display("-----------------------------------------");
    `endif
    `echo(a_mant);
    `echo(b_mant);
    `echo(a_expo);
    `echo(b_expo);
    `echo(a_is_sub);
    `echo(b_is_sub);
    `echo(a_is_n0);
    `echo(b_is_n0);
    `echo(a_is_nan);
    `echo(b_is_nan);
    `echo(r_isnan);
    `echo(is_inf_nan);
    `echo(r_is0nan);
    `echo(a_is_nor);
    `echo(rnd_out);
    `echo(a_is_nor);
    `echo(b_is_nor);
    `echo(status_nv);
    `echo(u_fixcal.a_is_s_nan);
    `echo(u_fixcal.b_is_s_nan);
    `echo(u_fixcal.r_is0nan);
    `echo(rnd);
end
`endif


endmodule