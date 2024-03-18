module mul_short_judge #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(
    input  logic                             a_is_sub    ,
    input  logic                             b_is_sub    ,
    input  logic [MANT_W-1 : 0]              a_mant      ,       
    input  logic [MANT_W-1 : 0]              b_mant      ,
    output logic [MANT_W   : 0]              mant_input  
);
assign mant_input = (a_is_sub && (!b_is_sub)) ? {1'b0,b_mant}     :
                    ((!a_is_sub) && b_is_sub) ? {1'b0,a_mant}     :
                    24'hffffff                                    ;
    
endmodule