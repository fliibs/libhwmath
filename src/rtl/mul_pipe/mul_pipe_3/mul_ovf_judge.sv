module mul_ovf_judge #(
    parameter  int unsigned EXPO_W = 8,
    localparam int unsigned EXPO_R = (1<<EXPO_W)-1
)(
    input  logic[EXPO_W + 1 : 0] expo_2,
    input  logic[EXPO_W + 1 : 0] expo_3,
    output logic                 overflow  
);
assign  overflow  =  (expo_2 >= EXPO_R) || (expo_3 >= EXPO_R)  ;
endmodule