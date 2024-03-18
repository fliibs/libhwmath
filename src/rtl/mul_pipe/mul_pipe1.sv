module mul_pipe1#(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(
    input  logic [SIGN_W + EXPO_W + MANT_W - 1 : 0] a         ,
    input  logic [SIGN_W + EXPO_W + MANT_W - 1 : 0] b         ,
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
    output logic                                    a_is_q    ,
    output logic                                    b_is_q    ,
    output logic                                    r_isnan   ,
    output logic                                    is_inf_nan,
    output logic                                    r_is0nan  
);
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
    .a_sign  (a_sign),
    .b_sign  (b_sign),
    .a_expo  (a_expo),
    .b_expo  (b_expo),
    .a_mant  (a_mant),
    .b_mant  (b_mant)
);

//-----------------------------
//fix cal
//-----------------------------
mul_fixcal #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_fixcal(
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

endmodule