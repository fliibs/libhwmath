module mul_ecp#(
    parameter  int unsigned SIGN_W =1,
    parameter  int unsigned EXPO_W = 8,
    parameter  int unsigned MANT_W = 23
)(
    
    input  logic       a_is_q     , 
    input  logic       b_is_q     ,
    input  logic       a_is_nan   ,
    input  logic       b_is_nan   ,
    input  logic       r_is_0nan  , 
    input  logic       overflow   ,
    input  logic       underflow  ,
    input  logic       inexact_rnd,
    input  logic       inexact_sft,
    output logic [4:0] status   
);
//------internal signals 
logic a_is_s_nan;
logic b_is_s_nan;
//------output Invalid Operation
assign a_is_s_nan =a_is_nan&&(!a_is_q);
assign b_is_s_nan =b_is_nan&&(!b_is_q);

assign status[4]=a_is_nan||b_is_nan||r_is_0nan;

//------output Divided by zero
assign status[3]=0;

//------output Overflow
assign status[2]=overflow;

//------output underflow
assign status[1]=underflow;

//------output Inexact
assign status[0]=inexact_rnd|inexact_sft;

endmodule