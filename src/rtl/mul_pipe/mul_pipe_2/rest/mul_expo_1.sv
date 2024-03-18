module mul_expo_1#(
    parameter   int unsigned SIGN_W = 1,
    parameter   int unsigned EXPO_W = 8,
    parameter   int unsigned MANT_W = 23,
    localparam  int unsigned EXPO_R = (1<<(EXPO_W-1))-1  
)(
    input  logic                 a_sign  ,
    input  logic                 b_sign  ,
    input  logic                 a_is_sub,
    input  logic                 b_is_sub,
    input  logic [EXPO_W-1:0]    a_expo  ,
    input  logic [EXPO_W-1:0]    b_expo  ,
    output logic                 sign_1  ,
    output logic [EXPO_W+1:0]    expo_1
);
    assign sign_1 = a_sign ^ b_sign                                 ;
    assign expo_1 = a_expo + b_expo -EXPO_R + !a_is_sub + !b_is_sub  ;
endmodule