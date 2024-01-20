// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Valu__Syms.h"


//======================

void Valu::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addInitCb(&traceInit, __VlSymsp);
    traceRegister(tfp->spTrace());
}

void Valu::traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->module(vlSymsp->name());
    tracep->scopeEscape(' ');
    Valu::traceInitTop(vlSymsp, tracep);
    tracep->scopeEscape('.');
}

//======================


void Valu::traceInitTop(void* userp, VerilatedVcd* tracep) {
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceInitSub0(userp, tracep);
    }
}

void Valu::traceInitSub0(void* userp, VerilatedVcd* tracep) {
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    const int c = vlSymsp->__Vm_baseCode;
    if (false && tracep && c) {}  // Prevent unused
    // Body
    {
        tracep->declBit(c+6,"clk", false,-1);
        tracep->declBit(c+7,"rst", false,-1);
        tracep->declBus(c+8,"op_in", false,-1, 1,0);
        tracep->declBus(c+9,"a_in", false,-1, 5,0);
        tracep->declBus(c+10,"b_in", false,-1, 5,0);
        tracep->declBit(c+11,"in_valid", false,-1);
        tracep->declBus(c+12,"out", false,-1, 5,0);
        tracep->declBit(c+13,"out_valid", false,-1);
        tracep->declBus(c+14,"alu WIDTH", false,-1, 31,0);
        tracep->declBit(c+6,"alu clk", false,-1);
        tracep->declBit(c+7,"alu rst", false,-1);
        tracep->declBus(c+8,"alu op_in", false,-1, 1,0);
        tracep->declBus(c+9,"alu a_in", false,-1, 5,0);
        tracep->declBus(c+10,"alu b_in", false,-1, 5,0);
        tracep->declBit(c+11,"alu in_valid", false,-1);
        tracep->declBus(c+12,"alu out", false,-1, 5,0);
        tracep->declBit(c+13,"alu out_valid", false,-1);
        tracep->declBus(c+1,"alu op_in_r", false,-1, 1,0);
        tracep->declBus(c+2,"alu a_in_r", false,-1, 5,0);
        tracep->declBus(c+3,"alu b_in_r", false,-1, 5,0);
        tracep->declBit(c+4,"alu in_valid_r", false,-1);
        tracep->declBus(c+5,"alu result", false,-1, 5,0);
    }
}

void Valu::traceRegister(VerilatedVcd* tracep) {
    // Body
    {
        tracep->addFullCb(&traceFullTop0, __VlSymsp);
        tracep->addChgCb(&traceChgTop0, __VlSymsp);
        tracep->addCleanupCb(&traceCleanup, __VlSymsp);
    }
}

void Valu::traceFullTop0(void* userp, VerilatedVcd* tracep) {
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceFullSub0(userp, tracep);
    }
}

void Valu::traceFullSub0(void* userp, VerilatedVcd* tracep) {
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        tracep->fullCData(oldp+1,(vlTOPp->alu__DOT__op_in_r),2);
        tracep->fullCData(oldp+2,(vlTOPp->alu__DOT__a_in_r),6);
        tracep->fullCData(oldp+3,(vlTOPp->alu__DOT__b_in_r),6);
        tracep->fullBit(oldp+4,(vlTOPp->alu__DOT__in_valid_r));
        tracep->fullCData(oldp+5,(vlTOPp->alu__DOT__result),6);
        tracep->fullBit(oldp+6,(vlTOPp->clk));
        tracep->fullBit(oldp+7,(vlTOPp->rst));
        tracep->fullCData(oldp+8,(vlTOPp->op_in),2);
        tracep->fullCData(oldp+9,(vlTOPp->a_in),6);
        tracep->fullCData(oldp+10,(vlTOPp->b_in),6);
        tracep->fullBit(oldp+11,(vlTOPp->in_valid));
        tracep->fullCData(oldp+12,(vlTOPp->out),6);
        tracep->fullBit(oldp+13,(vlTOPp->out_valid));
        tracep->fullIData(oldp+14,(6U),32);
    }
}
