module add_top#(
    parameter int unsigned SIGN_W=1,
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23
) (
    input  logic                                        clk ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     a   ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     b   ,
    input  logic  [1 : 0]                               rnd ,
    output logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     res       
);

logic [SIGN_W + EXPO_W + MANT_W -1 : 0] res_com;

add #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)
u_add(
    .a(a),
    .b(b),
    .rnd(rnd),
    .res(res_com)
);
always_ff @(posedge clk) begin 
    res<=res_com;
end
endmodule