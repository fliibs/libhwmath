`include "macro_defs.sv"
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
logic                               a_sign      ;              
logic   [EXPO_W - 1 : 0]            a_expo      ;
logic   [MANT_W - 1 : 0]            a_mant      ;
logic                               b_sign      ;
logic   [EXPO_W - 1 : 0]            b_expo      ;
logic   [MANT_W - 1 : 0]            b_mant      ;
logic                               r_isnan     ;
logic                               is_inf_nan  ;
logic                               r_is0nan    ;
logic   [1 : 0]                     rnd_1       ;   

logic                               a_is_nor    ;
logic                               b_is_nor    ;
logic                               status_nv   ;

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

logic                               a_nan_2     ;
logic                               b_nan_2     ;
logic                               a_n0_2      ;
logic                               b_n0_2      ;
logic                               a_q_2       ;
logic                               b_q_2       ;
logic                               r_nan_2     ;
logic                               inf_nan_2   ;
logic                               r_0nan_2    ;
logic   [1 : 0]                     rnd_2       ;

logic                               a_sign_2    ;              
logic   [EXPO_W - 1 : 0]            a_expo_2    ;
logic   [MANT_W - 1 : 0]            a_mant_2    ;
logic                               b_sign_2    ;
logic   [EXPO_W - 1 : 0]            b_expo_2    ;
logic   [MANT_W - 1 : 0]            b_mant_2    ; 

logic                               a_is_nor_2  ;
logic                               b_is_nor_2  ;
logic                               status_nv_2 ;
//-------------------------------------//
//pipeline 1
//-------------------------------------//
mul_pipe1#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_pipe1(
    .a         (a           ),
    .b         (b           ),
    .rnd       (rnd         ),
    `ifdef REG_1
    .clk       (clk         ),
    `endif
    .a_sign    (a_sign      ),
    .b_sign    (b_sign      ),
    .a_expo    (a_expo      ),
    .b_expo    (b_expo      ),
    .a_mant    (a_mant      ),
    .b_mant    (b_mant      ),
    .a_is_sub  (a_is_sub    ),
    .b_is_sub  (b_is_sub    ),
    .a_is_n0   (a_is_n0     ),
    .b_is_n0   (b_is_n0     ),
    .a_is_nan  (a_is_nan    ),
    .b_is_nan  (b_is_nan    ),
    .r_isnan   (r_isnan     ),
    .is_inf_nan(is_inf_nan  ),
    .r_is0nan  (r_is0nan    ),
    .rnd_out   (rnd_1       ),
    .a_is_nor  (a_is_nor    ),
    .b_is_nor  (b_is_nor    ),
    .status_nv (status_nv   )
);

//-------------------------------------//
//pipeline 2
//-------------------------------------//
mul_pipe2#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W) 
)u_mul_pipe2(
    `ifdef REG_2
    .clk          (clk         ),
    `endif 
    .a_sign       (a_sign      ),              
    .a_expo       (a_expo      ),
    .a_mant       (a_mant      ),
    .b_sign       (b_sign      ),
    .b_expo       (b_expo      ),
    .b_mant       (b_mant      ),   
    .a_sub        (a_is_sub    ),              
    .b_sub        (b_is_sub    ), 
    .a_nan_in     (a_is_nan    ),   
    .b_nan_in     (b_is_nan    ),   
    .a_n0_in      (a_is_n0     ),   
    .b_n0_in      (b_is_n0     ),     
    .r_nan_in     (r_isnan     ),   
    .inf_nan_in   (is_inf_nan  ),   
    .r_0nan_in    (r_is0nan    ),   
    .rnd_in       (rnd_1       ), 
    .a_is_nor_in  (a_is_nor    ),    
    .b_is_nor_in  (b_is_nor    ),    
    .status_nv_in (status_nv   ),    
    .a_nan_out    (a_nan_2     ),
    .b_nan_out    (b_nan_2     ),
    .a_n0_out     (a_n0_2      ),
    .b_n0_out     (b_n0_2      ),
    .a_q_out      (a_q_2       ),
    .b_q_out      (b_q_2       ),
    .r_nan_out    (r_nan_2     ),
    .inf_nan_out  (inf_nan_2   ),
    .r_0nan_out   (r_0nan_2    ),
    .rnd_out      (rnd_2       ),
    .a_sign_out   (a_sign_2    ),              
    .a_expo_out   (a_expo_2    ),
    .a_mant_out   (a_mant_2    ),
    .b_sign_out   (b_sign_2    ),
    .b_expo_out   (b_expo_2    ),
    .b_mant_out   (b_mant_2    ), 
    .mant_1       (mant_1      ),
    .sign_1       (sign_1      ),
    .expo_1       (expo_1      ),
    .zero_nums_uc (zero_nums_uc),
    .mask_short   (mask_short  ),
    .r_shift      (r_shift     ),
    .l_shift      (l_shift     ),
    .a_is_nor_out (a_is_nor_2  ),    
    .b_is_nor_out (b_is_nor_2  ),    
    .status_nv_out(status_nv_2 )
);
//-------------------------------------//
//pipeline 3
//-------------------------------------//
mul_pipe3#(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_mul_pipe3(
    `ifdef REG_3
    .clk              (clk         ),
    `endif
    .mask_short       (mask_short  ), 
    .sign_1           (sign_1      ),  
    .expo_1           (expo_1      ), 
    .mant_1           (mant_1      ), 
    .zero_nums_uc     (zero_nums_uc), 
    .r_shift          (r_shift     ), 
    .l_shift          (l_shift     ), 
    .rnd              (rnd_2       ), 
    .a_q              (a_q_2       ),
    .b_q              (b_q_2       ),
    .a_n0             (a_n0_2      ),   
    .b_n0             (b_n0_2      ),          
    .a_nan            (a_nan_2     ), 
    .b_nan            (b_nan_2     ), 
    .r_nan            (r_nan_2     ), 
    .r_0nan           (r_0nan_2    ),  
    .a_sign           (a_sign_2    ),               
    .a_expo           (a_expo_2    ), 
    .a_mant           (a_mant_2    ), 
    .b_sign           (b_sign_2    ), 
    .b_expo           (b_expo_2    ), 
    .b_mant           (b_mant_2    ),  
    .inf_nan          (inf_nan_2   ),  
    .a_is_nor         (a_is_nor_2  ),
    .b_is_nor         (b_is_nor_2  ),
    .status_nv        (status_nv_2 ),  
    .status           (status      ), 
    .res              (res         )  
);
endmodule