// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Valu.h for the primary calling header

#include "Valu.h"
#include "Valu__Syms.h"

//==========

VL_CTOR_IMP(Valu) {
    Valu__Syms* __restrict vlSymsp = __VlSymsp = new Valu__Syms(this, name());
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    VL_CELL(__PVT____024unit, Valu___024unit);
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Valu::__Vconfigure(Valu__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Valu::~Valu() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Valu::_settle__TOP__2(Valu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu::_settle__TOP__2\n"); );
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->alu__DOT__result = 0U;
    if (vlTOPp->alu__DOT__in_valid_r) {
        vlTOPp->alu__DOT__result = (0x3fU & ((1U == (IData)(vlTOPp->alu__DOT__op_in_r))
                                              ? ((IData)(vlTOPp->alu__DOT__a_in_r) 
                                                 + (IData)(vlTOPp->alu__DOT__b_in_r))
                                              : ((2U 
                                                  == (IData)(vlTOPp->alu__DOT__op_in_r))
                                                  ? 
                                                 ((IData)(1U) 
                                                  + 
                                                  ((IData)(vlTOPp->alu__DOT__a_in_r) 
                                                   + 
                                                   (~ (IData)(vlTOPp->alu__DOT__b_in_r))))
                                                  : 0U)));
    }
}

void Valu::_eval_initial(Valu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu::_eval_initial\n"); );
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst = vlTOPp->rst;
}

void Valu::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu::final\n"); );
    // Variables
    Valu__Syms* __restrict vlSymsp = this->__VlSymsp;
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Valu::_eval_settle(Valu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu::_eval_settle\n"); );
    Valu* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__2(vlSymsp);
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    vlTOPp->__Vm_traceActivity[0U] = 1U;
}

void Valu::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Valu::_ctor_var_reset\n"); );
    // Body
    clk = VL_RAND_RESET_I(1);
    rst = VL_RAND_RESET_I(1);
    op_in = VL_RAND_RESET_I(2);
    a_in = VL_RAND_RESET_I(6);
    b_in = VL_RAND_RESET_I(6);
    in_valid = VL_RAND_RESET_I(1);
    out = VL_RAND_RESET_I(6);
    out_valid = VL_RAND_RESET_I(1);
    alu__DOT__op_in_r = VL_RAND_RESET_I(2);
    alu__DOT__a_in_r = VL_RAND_RESET_I(6);
    alu__DOT__b_in_r = VL_RAND_RESET_I(6);
    alu__DOT__in_valid_r = VL_RAND_RESET_I(1);
    alu__DOT__result = VL_RAND_RESET_I(6);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            __Vm_traceActivity[__Vi0] = VL_RAND_RESET_I(1);
    }}
}
