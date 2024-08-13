module fp_min #(
    parameter int unsigned SIGN_W=1,
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23
) (
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     ina ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     inb ,
    input  logic  [1 : 0]                               rnd ,
    output logic  [4 : 0]                               status,
    output logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     res 
);

logic                   a_sign;
logic [EXPO_W - 1 : 0]  a_expo;
logic [MANT_W - 1 : 0]  a_mant;
logic                   b_sign;
logic [EXPO_W - 1 : 0]  b_expo;
logic [MANT_W - 1 : 0]  b_mant;

logic   a_is_nan    ;
logic   a_is_snan   ;
logic   a_is_zero   ;
logic   a_is_inf    ;
logic   a_is_inf_pos;
logic   a_is_inf_neg;
logic   a_is_nor    ;

logic   b_is_nan    ;
logic   b_is_snan   ;
logic   b_is_zero   ;
logic   b_is_inf    ;
logic   b_is_inf_pos;
logic   b_is_inf_neg;
logic   b_is_nor    ;


fp_unpack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_unpack_a (
    .ind     (ina),
    .ind_sign(a_sign),
    .ind_expo(a_expo),
    .ind_mant(a_mant)
);

fp_unpack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_unpack_b (
    .ind     (inb),
    .ind_sign(b_sign),
    .ind_expo(b_expo),
    .ind_mant(b_mant)
);

fp_classify #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_fp_classify_a (
    .ind       (ina),
    .is_nan    (a_is_nan    ),
    .is_snan   (a_is_snan   ),
    .is_zero   (a_is_zero   ),
    .is_inf    (a_is_inf    ),
    .is_inf_pos(a_is_inf_pos),
    .is_inf_neg(a_is_inf_neg),
    .is_nor    (a_is_nor    )
);

fp_classify #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_fp_classify_b (
    .ind       (inb),
    .is_nan    (b_is_nan    ),
    .is_snan   (b_is_snan   ),
    .is_zero   (b_is_zero   ),
    .is_inf    (b_is_inf    ),
    .is_inf_pos(b_is_inf_pos),
    .is_inf_neg(b_is_inf_neg),
    .is_nor    (b_is_nor    )
);

min_res_gen #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_min_res_gen(
    .ina         (ina),
    .a_sign      (a_sign      ),
    .a_expo      (a_expo      ),
    .a_mant      (a_mant      ),
    .a_is_nan    (a_is_nan    ),
    .a_is_snan   (a_is_snan   ),
    .a_is_zero   (a_is_zero   ),
    .a_is_inf    (a_is_inf    ),
    .a_is_inf_pos(a_is_inf_pos),
    .a_is_inf_neg(a_is_inf_neg),
    .a_is_nor    (a_is_nor    ),
    .inb         (inb),
    .b_sign      (b_sign      ),
    .b_expo      (b_expo      ),
    .b_mant      (b_mant      ),
    .b_is_nan    (b_is_nan    ),
    .b_is_snan   (b_is_snan   ),
    .b_is_zero   (b_is_zero   ),
    .b_is_inf    (b_is_inf    ),
    .b_is_inf_pos(b_is_inf_pos),
    .b_is_inf_neg(b_is_inf_neg),
    .b_is_nor    (b_is_nor    ),
    .res         (res         ),
    .status      (status      )
);

endmodule