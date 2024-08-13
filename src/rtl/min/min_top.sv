module min_top#(
    parameter int unsigned SIGN_W=1,
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23
) (
    input  logic                                        clk ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     a   ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     b   ,
    input  logic  [1 : 0]                               rnd ,
    output logic  [4 : 0]                               status,
    output logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     res       
);

logic [SIGN_W + EXPO_W + MANT_W -1 : 0] res_tmp;

fp_min #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)
u_fp_min(
    .ina(a),
    .inb(b),
    .rnd(rnd),
    .status(status),
    .res(res_tmp)
);
always_ff @(posedge clk) begin 
    res<=res_tmp;
end
endmodule