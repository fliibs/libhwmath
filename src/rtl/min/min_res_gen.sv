module min_res_gen #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
) (
    input logic [SIGN_W + EXPO_W + MANT_W -1 : 0]   ina         ,
    input logic [SIGN_W - 1 : 0]                    a_sign      ,
    input logic [EXPO_W - 1 : 0]                    a_expo      ,
    input logic [MANT_W - 1 : 0]                    a_mant      ,
    input logic                                     a_is_nan    ,
    input logic                                     a_is_snan   ,
    input logic                                     a_is_zero   ,
    input logic                                     a_is_inf    ,
    input logic                                     a_is_inf_pos,
    input logic                                     a_is_inf_neg,
    input logic                                     a_is_nor    ,

    input logic [SIGN_W + EXPO_W + MANT_W -1 : 0]   inb         ,
    input logic [SIGN_W - 1 : 0]                    b_sign      ,
    input logic [EXPO_W - 1 : 0]                    b_expo      ,
    input logic [MANT_W - 1 : 0]                    b_mant      ,
    input logic                                     b_is_nan    ,
    input logic                                     b_is_snan   ,
    input logic                                     b_is_zero   ,
    input logic                                     b_is_inf    ,
    input logic                                     b_is_inf_pos,
    input logic                                     b_is_inf_neg,
    input logic                                     b_is_nor    ,
    output logic  [4 : 0]                           status,
    output logic [SIGN_W + EXPO_W + MANT_W -1 : 0 ] res
);

logic a_min;
logic in_equal;
logic in_is_nan;

logic [1:0] in_sign;

assign in_sign = {a_sign,b_sign};
assign in_is_nan = a_is_nan || b_is_nan;

always_comb begin : compare_logic
    a_min = 1'b0;
    in_equal = 1'b0;
    if (in_is_nan) begin
        if (a_is_nan && b_is_nan) a_min = 1'b1;
        else if (a_is_nan) a_min = 1'b0;
        else if (b_is_nan) a_min = 1'b1;
    end else if (a_is_inf && b_is_inf) begin
        case (in_sign)
            2'b00:  a_min = 1'b1;
            2'b01:  a_min = 1'b0;
            2'b10:  a_min = 1'b1;
            2'b11:  a_min = 1'b0;
        endcase
    end else if (a_is_inf && (!b_is_inf)) begin
        a_min = a_sign ? 1'b1 : 1'b0;
    end else if ((!a_is_inf) && b_is_inf) begin
        a_min = b_sign ? 1'b0 : 1'b1;
    end else begin
        if (a_is_zero && b_is_zero) in_equal = 1'b1;
        else if (a_sign != b_sign) a_min = (a_sign < b_sign) ? 1'b0:1'b1;
        else if (a_sign == b_sign) begin
            if(a_expo != b_expo) begin
                if(!a_sign) begin
                    a_min = (a_expo < b_expo) ? 1'b1:1'b0;
                end else begin
                    a_min = (a_expo < b_expo) ? 1'b0:1'b1;
                end
            end else begin
                if(!a_sign) begin 
                    a_min = (a_mant < b_mant) ? 1'b1:1'b0;
                end else begin
                    a_min = (a_mant < b_mant) ? 1'b0:1'b1;
                end
            end
        end
    end
end

assign res = in_equal ? ina : (a_min ? ina : inb);
assign status = a_is_snan || b_is_snan;


endmodule