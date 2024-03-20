module mul_ecp(
    input  logic       is_inf_nan ,
    input  logic       r_is_nan   ,
    input  logic       a_is_n0    ,
    input  logic       b_is_n0    ,
    input  logic       a_is_nor   ,
    input  logic       b_is_nor   ,
    input  logic       status_nv  ,
    input  logic       overflow   ,
    input  logic       underflow  ,
    input  logic       inexact_rnd,
    input  logic       inexact_sft,
    output logic [4:0] status     
);
//------internal signals 
logic  inexact   ;
logic  inexact_of;
//------output Invalid Operation
assign status[4]=status_nv;

//------output Divided by zero
assign status[3]=1'b0;

//------output Overflow
assign status[2]=overflow && (!is_inf_nan); 

//------output underflow
assign status[1]=underflow && a_is_n0 && b_is_n0;

//------output Inexact
assign inexact_of= a_is_nor && b_is_nor && overflow;
assign inexact   = inexact_rnd || inexact_sft;
assign status[0] = (inexact && !r_is_nan) || inexact_of;

endmodule