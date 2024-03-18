module mul_pipe_top#(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23,
    localparam int unsigned ZERO_D = $clog2(MANT_W + 1)
)(
    input  logic                                        clk   ,
    input  logic [SIGN_W + EXPO_W + MANT_W - 1 : 0 ]    a     ,
    input  logic [SIGN_W + EXPO_W + MANT_W - 1 : 0 ]    b     ,
    input  logic [1 : 0 ]                               rnd   ,
    output logic [4 : 0 ]                               status,
    output logic [SIGN_W + EXPO_W + MANT_W - 1 : 0 ]    res  
);

//-------------------------------------//
//pipe1 signals
//-------------------------------------//
logic                               a_is_sub    ;
logic                               b_is_sub    ;
logic                               a_is_nan    ;
logic                               b_is_nan    ;
logic                               a_is_n0     ;
logic                               b_is_n0     ;
logic                               a_is_q      ;
logic                               b_is_q      ;
logic                               a_sign      ;              
logic   [EXPO_W - 1 : 0]            a_expo      ;
logic   [MANT_W - 1 : 0]            a_mant      ;
logic                               b_sign      ;
logic   [EXPO_W - 1 : 0]            b_expo      ;
logic   [MANT_W - 1 : 0]            b_mant      ;
logic                               r_isnan     ;
logic                               is_inf_nan  ;
logic                               r_is0nan    ;

//---------pipe1 reg signals
logic                               a_sign_reg1 ;              
logic   [EXPO_W - 1 : 0]            a_expo_reg1 ;
logic   [MANT_W - 1 : 0]            a_mant_reg1 ;
logic                               b_sign_reg1 ;
logic   [EXPO_W - 1 : 0]            b_expo_reg1 ;
logic   [MANT_W - 1 : 0]            b_mant_reg1 ;   
logic                               a_sub_reg1  ;              
logic                               b_sub_reg1  ;              
logic                               a_nan_reg1  ;
logic                               b_nan_reg1  ;
logic                               a_q_reg1    ;
logic                               b_q_reg1    ;
logic                               a_n0_reg1   ;
logic                               b_n0_reg1   ;
logic                               r_nan_reg1  ;
logic                               inf_nan_reg1;
logic                               r_0nan_reg1 ;
logic   [1 : 0]                     rnd_reg1    ;
//-------------------------------------//
//pipeline 2 signals
//-------------------------------------//
logic                               sign_1      ;
logic   [EXPO_W + 1 : 0]            expo_1      ;
logic   [2*MANT_W + 1 : 0]          mant_1      ;
logic   [$clog2(MANT_W + 1) -1: 0]  zero_nums_uc;
logic   [MANT_W : 0]                mask_short  ;
logic   [ZERO_D : 0]                r_shift     ;
logic   [ZERO_D : 0]                l_shift     ;
//---------pipe2 reg signals
logic                               sign_1_reg2    ;
logic   [EXPO_W + 1 : 0]            expo_1_reg2    ;
logic   [2*MANT_W + 1 : 0]          mant_1_reg2    ;
logic   [$clog2(MANT_W + 1) -1: 0]  z_nums_uc_reg2 ;
logic   [MANT_W : 0]                mask_short_reg2;
logic   [ZERO_D : 0]                r_shift_reg2   ;
logic   [ZERO_D : 0]                l_shift_reg2   ;

logic                               a_nan_reg2  ;
logic                               b_nan_reg2  ;
logic                               a_n0_reg2   ;
logic                               b_n0_reg2   ;
logic                               a_q_reg2    ;
logic                               b_q_reg2    ;
logic                               r_nan_reg2  ;
logic                               inf_nan_reg2;
logic                               r_0nan_reg2 ;
logic   [1 : 0]                     rnd_reg2    ;

logic                               a_sign_reg2 ;              
logic   [EXPO_W - 1 : 0]            a_expo_reg2 ;
logic   [MANT_W - 1 : 0]            a_mant_reg2 ;
logic                               b_sign_reg2 ;
logic   [EXPO_W - 1 : 0]            b_expo_reg2 ;
logic   [MANT_W - 1 : 0]            b_mant_reg2 ;   


//-------------------------------------//
//pipeline 1
//-------------------------------------//
mul_pipe1#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_pipe1(
    .a         (a),
    .b         (b),
    .a_sign    (a_sign),
    .b_sign    (b_sign),
    .a_expo    (a_expo),
    .b_expo    (b_expo),
    .a_mant    (a_mant),
    .b_mant    (b_mant),
    .a_is_sub  (a_is_sub),
    .b_is_sub  (b_is_sub),
    .a_is_n0   (a_is_n0),
    .b_is_n0   (b_is_n0),
    .a_is_nan  (a_is_nan),
    .b_is_nan  (b_is_nan),
    .a_is_q    (a_is_q),
    .b_is_q    (b_is_q),
    .r_isnan   (r_isnan),
    .is_inf_nan(is_inf_nan),
    .r_is0nan  (r_is0nan)
);
always_ff @( posedge clk) begin 
    a_sign_reg1 <= a_sign       ;
    a_expo_reg1 <= a_expo       ;
    a_mant_reg1 <= a_mant       ;
    b_sign_reg1 <= b_sign       ;
    b_expo_reg1 <= b_expo       ;
    b_mant_reg1 <= b_mant       ;
    a_sub_reg1  <= a_is_sub     ;
    b_sub_reg1  <= b_is_sub     ;
    a_nan_reg1  <= a_is_nan     ;
    b_nan_reg1  <= b_is_nan     ; 
    a_n0_reg1   <= a_is_n0      ; 
    b_n0_reg1   <= b_is_n0      ;
    a_q_reg1    <= a_is_q       ;
    b_q_reg1    <= b_is_q       ;
    r_nan_reg1  <= r_isnan      ;
    inf_nan_reg1<= is_inf_nan   ;
    r_0nan_reg1 <= r_is0nan     ;
    rnd_reg1    <= rnd          ;
end
//-------------------------------------//
//pipeline 2
//-------------------------------------//
mul_pipe2#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_pipe2(
    .a_sign_reg1 (a_sign_reg1),              
    .a_expo_reg1 (a_expo_reg1),
    .a_mant_reg1 (a_mant_reg1),
    .b_sign_reg1 (b_sign_reg1),
    .b_expo_reg1 (b_expo_reg1),
    .b_mant_reg1 (b_mant_reg1),   
    .a_sub_reg1  (a_sub_reg1),              
    .b_sub_reg1  (b_sub_reg1), 
    .mant_1      (mant_1),
    .sign_1      (sign_1),
    .expo_1      (expo_1),
    .zero_nums_uc(zero_nums_uc),
    .mask_short  (mask_short),
    .r_shift     (r_shift),
    .l_shift     (l_shift)
);

always_ff @(posedge clk) begin
    sign_1_reg2     <= sign_1       ;
    expo_1_reg2     <= expo_1       ;
    mant_1_reg2     <= mant_1       ;
    z_nums_uc_reg2  <= zero_nums_uc ;
    mask_short_reg2 <= mask_short   ;
    r_shift_reg2    <= r_shift      ;
    l_shift_reg2    <= l_shift      ;
    a_nan_reg2      <= a_nan_reg1   ;
    b_nan_reg2      <= b_nan_reg1   ;
    a_n0_reg2       <= a_n0_reg1    ;
    b_n0_reg2       <= b_n0_reg1    ;
    a_q_reg2        <= a_q_reg1     ;
    b_q_reg2        <= b_q_reg1     ;
    r_nan_reg2      <= r_0nan_reg1  ;
    inf_nan_reg2    <= inf_nan_reg1 ;
    r_0nan_reg2     <= r_0nan_reg1  ;
    rnd_reg2        <= rnd_reg1     ;
    a_sign_reg2     <= a_sign_reg1  ;
    a_expo_reg2     <= a_expo_reg1  ;
    a_mant_reg2     <= a_mant_reg1  ;
    b_sign_reg2     <= b_sign_reg1  ;
    b_expo_reg2     <= b_expo_reg1  ;
    b_mant_reg2     <= b_mant_reg1  ;
end

mul_pipe3#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_pipe3(
    .mask_short_reg2  (mask_short_reg2)  , 
    .sign_1_reg2      (sign_1_reg2)  ,  
    .expo_1_reg2      (expo_1_reg2)  , 
    .mant_1_reg2      (mant_1_reg2)  , 
    .z_nums_uc_reg2   (z_nums_uc_reg2)  , 
    .r_shift_reg2     (r_shift_reg2)  , 
    .l_shift_reg2     (l_shift_reg2)  , 
    .rnd_reg2         (rnd_reg2)  , 
    .a_q_reg2         (a_q_reg2)  ,
    .b_q_reg2         (b_q_reg2)  ,
    .a_n0_reg2        (a_n0_reg2)  ,   
    .b_n0_reg2        (b_n0_reg2)  ,          
    .a_nan_reg2       (a_nan_reg2)  , 
    .b_nan_reg2       (b_nan_reg2)  , 
    .r_nan_reg2       (r_nan_reg2)  , 
    .r_0nan_reg2      (r_0nan_reg2)  ,  
    .a_sign_reg2      (a_sign_reg2)  ,               
    .a_expo_reg2      (a_expo_reg2)  , 
    .a_mant_reg2      (a_mant_reg2)  , 
    .b_sign_reg2      (b_sign_reg2)  , 
    .b_expo_reg2      (b_expo_reg2)  , 
    .b_mant_reg2      (b_mant_reg2)  ,  
    .inf_nan_reg2     (inf_nan_reg2)  ,    
    .status           (status)  , 
    .res              (res)  
);

endmodule