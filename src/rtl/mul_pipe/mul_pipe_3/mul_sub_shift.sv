module mul_sub_shift #(
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23,
    parameter int unsigned ZERO_D=6
) (
    input  logic   [EXPO_W   + 1: 0]          expo_1      ,
    input  logic   [2*MANT_W + 1: 0]          mant_1      ,
    input  logic   [ZERO_D   - 1: 0]          zero_nums_c ,
    input  logic   [2*MANT_W + 1: 0]          mant_2_l    ,
    input  logic   [2*MANT_W + 1: 0]          mant_2_r    ,
    input  logic                              mant_2_r_rcd,
    input  logic   [2*MANT_W + 1: 0]          mant_2_l_zn ,
    output logic   [EXPO_W   + 1: 0]          expo_2      ,
    output logic   [2*MANT_W + 1: 0]          mant_2      ,
    output logic                              underflow   ,
    output logic                              inexact_sft ,
    output logic                              bit_s_record
);

always_comb begin:subnoral_shift
    bit_s_record        = 1'b0                 ;
    mant_2              = {(2*MANT_W+1){1'b0}} ;
    expo_2              = {(EXPO_W  +2){1'b0}} ;
    underflow           = 1'b0                 ;
    inexact_sft         = 1'b0                 ;
    if(mant_1[2*MANT_W + 1] && (!expo_1[EXPO_W + 1])) begin
        `ifndef SYN
        $display("into shift right");
        `endif
        expo_2          = expo_1 + 1'b1                ;
        mant_2          = {1'b0,mant_1[2*MANT_W+1 : 1]};
        bit_s_record    = mant_1[0]                    ;
    end
    else begin
        if((!expo_1[EXPO_W + 1]) && (expo_1[EXPO_W : 0] > {4'b0,zero_nums_c}) && (|mant_1)) begin
            `ifndef SYN
            $display("not into shift right");
            `endif
            expo_2      = expo_1 - zero_nums_c         ;
            mant_2      = mant_2_l_zn                  ;
        end
        else begin
            `ifndef SYN
            $display("into 0.xxxx and denormalized");
            `endif
            underflow   = 1'b1                         ;
            expo_2      = 10'b0                        ;
            if ((!expo_1[EXPO_W + 1]) && (|expo_1[EXPO_W : 0]))
                mant_2      = mant_2_l                 ;
            else 
            begin
                mant_2      = mant_2_r                 ; 
                bit_s_record= mant_2_r_rcd             ;
                inexact_sft = mant_2_r_rcd             ;
            end
        end
    end
end
endmodule