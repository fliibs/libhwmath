// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vmul_para.h for the primary calling header

#include "Vmul_para.h"
#include "Vmul_para__Syms.h"

//==========

VL_CTOR_IMP(Vmul_para) {
    Vmul_para__Syms* __restrict vlSymsp = __VlSymsp = new Vmul_para__Syms(this, name());
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void Vmul_para::__Vconfigure(Vmul_para__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-12);
    Verilated::timeprecision(-12);
}

Vmul_para::~Vmul_para() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void Vmul_para::_eval_initial(Vmul_para__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_eval_initial\n"); );
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vmul_para::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::final\n"); );
    // Variables
    Vmul_para__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vmul_para::_eval_settle(Vmul_para__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_eval_settle\n"); );
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    vlTOPp->__Vm_traceActivity[0U] = 1U;
}

void Vmul_para::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_ctor_var_reset\n"); );
    // Body
    a = VL_RAND_RESET_I(32);
    b = VL_RAND_RESET_I(32);
    rnd = VL_RAND_RESET_I(2);
    res = VL_RAND_RESET_I(32);
    mul_para__DOT__a_is_nan = VL_RAND_RESET_I(1);
    mul_para__DOT__b_is_nan = VL_RAND_RESET_I(1);
    mul_para__DOT__a_is_inf = VL_RAND_RESET_I(1);
    mul_para__DOT__b_is_inf = VL_RAND_RESET_I(1);
    mul_para__DOT__res_sign = VL_RAND_RESET_I(1);
    mul_para__DOT__res_expo = VL_RAND_RESET_I(8);
    mul_para__DOT__res_mant = VL_RAND_RESET_I(23);
    mul_para__DOT__sign_1 = VL_RAND_RESET_I(1);
    mul_para__DOT__expo_1 = VL_RAND_RESET_I(10);
    mul_para__DOT__mant_1 = VL_RAND_RESET_Q(48);
    mul_para__DOT__mant_input = VL_RAND_RESET_I(24);
    mul_para__DOT__mask_short = VL_RAND_RESET_I(24);
    mul_para__DOT__zero_nums_c = VL_RAND_RESET_I(6);
    mul_para__DOT__expo_2 = VL_RAND_RESET_I(10);
    mul_para__DOT__mant_2 = VL_RAND_RESET_Q(48);
    mul_para__DOT__bit_s_record = VL_RAND_RESET_I(1);
    mul_para__DOT__mant_rnd = VL_RAND_RESET_I(25);
    mul_para__DOT__expo_rnd = VL_RAND_RESET_I(10);
    mul_para__DOT__expo_3 = VL_RAND_RESET_I(10);
    mul_para__DOT__r_isnan = VL_RAND_RESET_I(1);
    mul_para__DOT__r_is0nan = VL_RAND_RESET_I(1);
    mul_para__DOT____Vcellout__u_lod__zero_nums = VL_RAND_RESET_I(5);
    mul_para__DOT__u_lod__DOT__z4 = VL_RAND_RESET_I(24);
    mul_para__DOT__u_lod__DOT__z3 = VL_RAND_RESET_I(24);
    mul_para__DOT__u_lod__DOT__z2 = VL_RAND_RESET_I(24);
    mul_para__DOT__u_lod__DOT__z1 = VL_RAND_RESET_I(24);
    mul_para__DOT__u_lod__DOT__z0 = VL_RAND_RESET_I(24);
    __Vchglast__TOP__mul_para__DOT__u_lod__DOT__z2 = VL_RAND_RESET_I(24);
    __Vchglast__TOP__mul_para__DOT__u_lod__DOT__z1 = VL_RAND_RESET_I(24);
    { int __Vi0=0; for (; __Vi0<2; ++__Vi0) {
            __Vm_traceActivity[__Vi0] = VL_RAND_RESET_I(1);
    }}
}
