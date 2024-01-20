// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Valu__Syms.h"


void Valu::traceChgTop0(void* userp, VerilatedVcd* tracep) {
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    {
        vlTOPp->traceChgSub0(userp, tracep);
    }
}

void Valu::traceChgSub0(void* userp, VerilatedVcd* tracep) {
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode + 1);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[1U])) {
            tracep->chgCData(oldp+0,(vlTOPp->alu__DOT__op_in_r),2);
            tracep->chgCData(oldp+1,(vlTOPp->alu__DOT__a_in_r),6);
            tracep->chgCData(oldp+2,(vlTOPp->alu__DOT__b_in_r),6);
            tracep->chgBit(oldp+3,(vlTOPp->alu__DOT__in_valid_r));
            tracep->chgCData(oldp+4,(vlTOPp->alu__DOT__result),6);
        }
        tracep->chgBit(oldp+5,(vlTOPp->clk));
        tracep->chgBit(oldp+6,(vlTOPp->rst));
        tracep->chgCData(oldp+7,(vlTOPp->op_in),2);
        tracep->chgCData(oldp+8,(vlTOPp->a_in),6);
        tracep->chgCData(oldp+9,(vlTOPp->b_in),6);
        tracep->chgBit(oldp+10,(vlTOPp->in_valid));
        tracep->chgCData(oldp+11,(vlTOPp->out),6);
        tracep->chgBit(oldp+12,(vlTOPp->out_valid));
    }
}

void Valu::traceCleanup(void* userp, VerilatedVcd* /*unused*/) {
    Valu__Syms* __restrict vlSymsp = static_cast<Valu__Syms*>(userp);
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlSymsp->__Vm_activity = false;
        vlTOPp->__Vm_traceActivity[0U] = 0U;
        vlTOPp->__Vm_traceActivity[1U] = 0U;
    }
}
