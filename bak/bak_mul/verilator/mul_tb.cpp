// Verilator Example
// Norbertas Kremeris 2021
#include <stdlib.h>
#include <bitset>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vmul_para.h"
#include "Vmul_para__Syms.h"
int main(int argc, char** argv, char** env) {
    Vmul_para *dut = new Vmul_para;
    dut->a=756123457;
    dut->b=756123457;
    std::cout << "Actual Input: " << std::bitset<32>(dut->a) << std::endl;
    std::cout << std::bitset<32>(dut->b) << std::endl;
    std::cout << std::bitset<32>(dut->rnd) << std::endl;
    dut->eval();
    // Verilated::timeInc(1);
    std::cout << "Actual output: " <<std::bitset<32>(dut->res) << std::endl;
    delete dut;
    exit(0);
}