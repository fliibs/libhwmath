module mul_shift_num #(
    parameter  int unsigned  EXPO_W=8,
    parameter  int unsigned  MANT_W=23,
    parameter  int unsigned  ZERO_D=6
) (
    input   logic   [EXPO_W + 1 : 0]    expo_1  ,
    output  logic   [ZERO_D : 0]        r_shift ,
    output  logic   [ZERO_D : 0]        l_shift
);
    logic   [EXPO_W + 1  : 0]           r_shift_unc ;
    logic   [EXPO_W + 1  : 0]           l_shift_unc ;
    
    assign r_shift_unc  = expo_1 - 10'b1;
    assign r_shift      = ((r_shift_unc[ZERO_D] && r_shift_unc[ZERO_D-1]) || (|r_shift_unc[EXPO_W+1:ZERO_D+1])) ? 6'b101111:r_shift_unc[ZERO_D:0];
    assign l_shift_unc  = 10'b1 - expo_1;
    assign l_shift      = ((l_shift_unc[ZERO_D] && l_shift_unc[ZERO_D-1]) || (|l_shift_unc[EXPO_W+1:ZERO_D+1])) ? 6'b101111:l_shift_unc[ZERO_D:0];
endmodule