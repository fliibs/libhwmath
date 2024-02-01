module mul_nan_judge #(
    parameter EXPO_W=8,
    parameter MANT_W=23
) (
    input  logic                     r_isnan    ,
    input  logic                     r_is0nan   ,
    input  logic                     a_is_nan   ,
    input  logic                     b_is_nan   ,
    input  logic                     sign_1     ,
    input  logic                     a_sign     ,
    input  logic                     b_sign     ,
    input  logic [EXPO_W - 1 : 0]    a_expo     ,
    input  logic [EXPO_W - 1 : 0]    b_expo     ,
    input  logic [MANT_W - 1 : 0]    a_mant     ,
    input  logic [MANT_W - 1 : 0]    b_mant     ,
    output logic                     sign_nan   ,
    output logic [MANT_W - 1 : 0]    mant_4      
);
always_comb begin: nan_judge 
    if(r_isnan) begin
        if({|a_expo,a_is_nan} >=  {|b_expo,b_is_nan}) begin
            mant_4      =  {1'b1,a_mant[MANT_W-2:0]};
            sign_nan    =  a_sign | r_is0nan        ;
        end
        else begin
            mant_4      =  {1'b1,b_mant[MANT_W-2:0]};
            sign_nan    =  b_sign | r_is0nan        ;
        end
    end
    else begin
        mant_4      =   23'b0                       ;
        sign_nan    =   r_is0nan | sign_1           ;
    end
end   
endmodule