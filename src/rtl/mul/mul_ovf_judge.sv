module mul_ovf_judge #(
    parameter EXPO_W=8
) (
    input  logic[EXPO_W + 1 : 0] expo_2,
    input  logic[EXPO_W + 1 : 0] expo_3,
    output logic                 overflow  
);
assign  overflow  =  (expo_2 >= 10'd255) || (expo_3 >= 10'd255)  ;
endmodule