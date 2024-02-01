module mul_res_mux #(
    parameter EXPO_W=8,
    parameter MANT_W=23
) (
    input  logic                is_inf_nan  ,
    input  logic                sign_nan    ,
    input  logic                overflow    ,
    input  logic [1:0]          rnd         ,
    input  logic                sign_1      ,
    input  logic [MANT_W-1:0]   mant_4      ,
    input  logic [MANT_W-1:0]   mant_3      ,
    input  logic [EXPO_W-1:0]   expo_3      ,
    output logic                res_sign    ,
    output logic [EXPO_W-1:0]   res_expo    ,
    output logic [MANT_W-1:0]   res_mant    
);
always_comb begin: res_mux
    if (is_inf_nan) begin
        res_sign    =   sign_nan                    ;
        res_expo    =   8'hff                       ;
        res_mant    =   mant_4                      ;                     
    end 
    else begin
        res_sign    =   sign_1                      ;
        if (overflow) begin
            res_expo =  (((rnd[1]) && (!sign_1)) || ((rnd[0]) && sign_1)) ? 8'd255 : 8'd254     ;
            res_mant =  (((rnd[1]) && (!sign_1)) || ((rnd[0]) && sign_1)) ? 23'b0  : 23'h7fffff ;
        end 
        else begin
            res_expo =  expo_3;
            res_mant =  mant_3;
        end
    end
end    
endmodule