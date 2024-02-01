module mul_sub_shift #(
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23,
    parameter int unsigned ZERO_D=6
) (
    input  logic   [EXPO_W + 1 : 0]            expo_1      ,
    input  logic   [2*MANT_W + 1 : 0]          mant_1      ,
    input  logic   [ZERO_D -1: 0]              zero_nums_c ,
    input  logic   [2*MANT_W + 1: 0]           mant_2_l    ,
    input  logic   [2*MANT_W + 1: 0]           mant_2_r    ,
    input  logic   [2*MANT_W + 1: 0]           mant_2_l_zn ,
    output logic   [EXPO_W + 1  : 0]           expo_2      ,
    output logic   [2*MANT_W + 1: 0]           mant_2      ,
    output logic                               bit_s_record
);

always_comb begin:subnoral_shift
    bit_s_record        = 1'b0   ;
    mant_2              = 47'b0  ;
    expo_2              = 10'b0  ;
    if(mant_1[2*MANT_W + 1] && (!expo_1[ EXPO_W + 1 ])) begin
        expo_2          = expo_1 + 1'b1                ;
        mant_2          = {1'b0,mant_1[2*MANT_W+1 : 1]};
        bit_s_record    = mant_1[0]                    ;
        // zero_num_sel    = 1'b0                         ;
    end
    else begin
        if((!expo_1[EXPO_W+1]) && (expo_1[EXPO_W : 0] > {4'b0,zero_nums_c}) && (|mant_1)) begin
            expo_2      = expo_1 - zero_nums_c         ;
            // mant_2      = mant_1 << zero_nums_c        ;
            mant_2      = mant_2_l_zn                  ;
            // zero_num_sel=1'b1                          ;

        end
        else begin
            // zero_num_sel=1'b0                          ;
            expo_2      = 10'b0                        ;
            if ((!expo_1[EXPO_W+1]) && (|expo_1[EXPO_W : 0]))
                mant_2  = mant_2_r                     ;
            else 
                mant_2  = mant_2_l                     ; 
        end
    end
end
endmodule