module mul_pack #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(
    input  logic                               res_sign    ,
    input  logic   [EXPO_W - 1 : 0]            res_expo    ,
    input  logic   [MANT_W - 1 : 0]            res_mant    ,
    output logic   [EXPO_W + MANT_W :0]        res
);
always_comb begin
    res = {res_sign,res_expo,res_mant};
end
endmodule