module mul_tb;
    parameter int unsigned SIGN_W = 1;
    parameter int unsigned EXPO_W = 8;
    parameter int unsigned MANT_W=23;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] a      ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] b      ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] c      ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] c_std  ;
    logic [1 : 0]                      rnd    ;
    string                             a_str  ;
    string                             b_str  ;
    string                             c_str  ;
    integer                            file_handle;
    integer                            file_handle_in;

    mul_para #(
      .SIGN_W(SIGN_W),
      .EXPO_W(EXPO_W),
      .MANT_W(MANT_W)
    )
    u_mul_para(
      .a(a),
      .b(b),
      .rnd(rnd),
      .res(c)
    );
    initial begin
        rnd=2'b11;
        if(!($value$plusargs("A=%s", a_str))) 
            $display("A is not successfully read in");
        if(!$sscanf(a_str,"%0b",a))
            $display("Error: Failed to convert string to integer");
        if(!($value$plusargs("B=%s", b_str)))
            $display("B is not successfully read in");
        if(!$sscanf(b_str,"%0b",b))
            $display("Error: Failed to convert string to integer");
        if(!($value$plusargs("C=%s", c_str)))
            $display("C is not successfully read in");
        if(!$sscanf(c_str,"%0b",c_std))
            $display("Error: Failed to convert string to integer");
        #5;
        file_handle=$fopen("mul_out.txt","w");
        if(file_handle!=0)
            $fdisplay(file_handle,"%b",c);
        else
            $display("fail to open file_handle");
        $fclose(file_handle);
        file_handle_in=$fopen("mul_in.txt","w");
        if(file_handle_in!=0)
            $fdisplay(file_handle_in,"%b %b",a,b);
        else
            $display("fail to open file_handle");
        $fclose(file_handle_in);
    end
initial begin
    $fsdbDumpfile("*.fsdb");
    $fsdbDumpvars(0);
end
endmodule