// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vmul_para.h for the primary calling header

#include "Vmul_para.h"
#include "Vmul_para__Syms.h"

//==========

void Vmul_para::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vmul_para::eval\n"); );
    Vmul_para__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        vlSymsp->__Vm_activity = true;
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("mul_para.sv", 19, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void Vmul_para::_eval_initial_loop(Vmul_para__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("mul_para.sv", 19, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void Vmul_para::_combo__TOP__1(Vmul_para__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_combo__TOP__1\n"); );
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->mul_para__DOT__u_lod__DOT__z1 = ((0U != 
                                              (3U & 
                                               (vlTOPp->mul_para__DOT__u_lod__DOT__z1 
                                                >> 0x16U)))
                                              ? vlTOPp->mul_para__DOT__u_lod__DOT__z2
                                              : (0xfffffcU 
                                                 & (vlTOPp->mul_para__DOT__u_lod__DOT__z2 
                                                    << 2U)));
    vlTOPp->mul_para__DOT__a_is_nan = ((0xffU == (0xffU 
                                                  & (vlTOPp->a 
                                                     >> 0x17U))) 
                                       & (0U != (0x7fffffU 
                                                 & vlTOPp->a)));
    vlTOPp->mul_para__DOT__b_is_nan = ((0xffU == (0xffU 
                                                  & (vlTOPp->b 
                                                     >> 0x17U))) 
                                       & (0U != (0x7fffffU 
                                                 & vlTOPp->b)));
    vlTOPp->mul_para__DOT__sign_1 = (1U & ((vlTOPp->a 
                                            ^ vlTOPp->b) 
                                           >> 0x1fU));
    vlTOPp->mul_para__DOT__a_is_inf = ((0xffU == (0xffU 
                                                  & (vlTOPp->a 
                                                     >> 0x17U))) 
                                       & (~ (IData)(
                                                    (0U 
                                                     != 
                                                     (0x7fffffU 
                                                      & vlTOPp->a)))));
    vlTOPp->mul_para__DOT__b_is_inf = ((0xffU == (0xffU 
                                                  & (vlTOPp->b 
                                                     >> 0x17U))) 
                                       & (~ (IData)(
                                                    (0U 
                                                     != 
                                                     (0x7fffffU 
                                                      & vlTOPp->b)))));
    vlTOPp->mul_para__DOT__expo_1 = (0x3ffU & (((((0xffU 
                                                   & (vlTOPp->a 
                                                      >> 0x17U)) 
                                                  + 
                                                  (0xffU 
                                                   & (vlTOPp->b 
                                                      >> 0x17U))) 
                                                 - (IData)(0x7fU)) 
                                                + (1U 
                                                   & (~ (IData)(
                                                                (0U 
                                                                 != 
                                                                 (0xffU 
                                                                  & (vlTOPp->a 
                                                                     >> 0x17U))))))) 
                                               + (1U 
                                                  & (~ (IData)(
                                                               (0U 
                                                                != 
                                                                (0xffU 
                                                                 & (vlTOPp->b 
                                                                    >> 0x17U))))))));
    vlTOPp->mul_para__DOT__mant_1 = (0xffffffffffffULL 
                                     & ((QData)((IData)(
                                                        (((IData)(
                                                                  (0U 
                                                                   != 
                                                                   (0xffU 
                                                                    & (vlTOPp->a 
                                                                       >> 0x17U)))) 
                                                          << 0x17U) 
                                                         | (0x7fffffU 
                                                            & vlTOPp->a)))) 
                                        * (QData)((IData)(
                                                          (((IData)(
                                                                    (0U 
                                                                     != 
                                                                     (0xffU 
                                                                      & (vlTOPp->b 
                                                                         >> 0x17U)))) 
                                                            << 0x17U) 
                                                           | (0x7fffffU 
                                                              & vlTOPp->b))))));
    vlTOPp->mul_para__DOT__mant_input = (((0U != (0xffU 
                                                  & (vlTOPp->a 
                                                     >> 0x17U))) 
                                          & (~ (IData)(
                                                       (0U 
                                                        != 
                                                        (0xffU 
                                                         & (vlTOPp->b 
                                                            >> 0x17U))))))
                                          ? (0x7fffffU 
                                             & vlTOPp->b)
                                          : (((~ (IData)(
                                                         (0U 
                                                          != 
                                                          (0xffU 
                                                           & (vlTOPp->a 
                                                              >> 0x17U))))) 
                                              & (0U 
                                                 != 
                                                 (0xffU 
                                                  & (vlTOPp->b 
                                                     >> 0x17U))))
                                              ? (0x7fffffU 
                                                 & vlTOPp->a)
                                              : 0xffffffU));
    vlTOPp->mul_para__DOT__r_is0nan = (((~ (IData)(
                                                   (0U 
                                                    != 
                                                    (0xffU 
                                                     & (vlTOPp->a 
                                                        >> 0x17U))))) 
                                        & (IData)(vlTOPp->mul_para__DOT__b_is_inf)) 
                                       | ((~ (IData)(
                                                     (0U 
                                                      != 
                                                      (0xffU 
                                                       & (vlTOPp->b 
                                                          >> 0x17U))))) 
                                          & (IData)(vlTOPp->mul_para__DOT__a_is_inf)));
    vlTOPp->mul_para__DOT__bit_s_record = 0U;
    if ((1U & ((IData)((vlTOPp->mul_para__DOT__mant_1 
                        >> 0x2fU)) & (~ ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                         >> 9U))))) {
        vlTOPp->mul_para__DOT__bit_s_record = (1U & (IData)(vlTOPp->mul_para__DOT__mant_1));
    }
    vlTOPp->mul_para__DOT__mask_short = ((0x800000U 
                                          & vlTOPp->mul_para__DOT__mant_input) 
                                         | ((0x400000U 
                                             & (((~ 
                                                  (vlTOPp->mul_para__DOT__mant_input 
                                                   >> 0x17U)) 
                                                 << 0x16U) 
                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                            | ((0x200000U 
                                                & (((~ (IData)(
                                                               (0U 
                                                                != 
                                                                (3U 
                                                                 & (vlTOPp->mul_para__DOT__mant_input 
                                                                    >> 0x16U))))) 
                                                    << 0x15U) 
                                                   & vlTOPp->mul_para__DOT__mant_input)) 
                                               | ((0x100000U 
                                                   & (((~ (IData)(
                                                                  (0U 
                                                                   != 
                                                                   (7U 
                                                                    & (vlTOPp->mul_para__DOT__mant_input 
                                                                       >> 0x15U))))) 
                                                       << 0x14U) 
                                                      & vlTOPp->mul_para__DOT__mant_input)) 
                                                  | ((0x80000U 
                                                      & (((~ (IData)(
                                                                     (0U 
                                                                      != 
                                                                      (0xfU 
                                                                       & (vlTOPp->mul_para__DOT__mant_input 
                                                                          >> 0x14U))))) 
                                                          << 0x13U) 
                                                         & vlTOPp->mul_para__DOT__mant_input)) 
                                                     | ((0x40000U 
                                                         & (((~ (IData)(
                                                                        (0U 
                                                                         != 
                                                                         (0x1fU 
                                                                          & (vlTOPp->mul_para__DOT__mant_input 
                                                                             >> 0x13U))))) 
                                                             << 0x12U) 
                                                            & vlTOPp->mul_para__DOT__mant_input)) 
                                                        | ((0x20000U 
                                                            & (((~ (IData)(
                                                                           (0U 
                                                                            != 
                                                                            (0x3fU 
                                                                             & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0x12U))))) 
                                                                << 0x11U) 
                                                               & vlTOPp->mul_para__DOT__mant_input)) 
                                                           | ((0x10000U 
                                                               & (((~ (IData)(
                                                                              (0U 
                                                                               != 
                                                                               (0x7fU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0x11U))))) 
                                                                   << 0x10U) 
                                                                  & vlTOPp->mul_para__DOT__mant_input)) 
                                                              | ((0x8000U 
                                                                  & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0xffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0x10U))))) 
                                                                      << 0xfU) 
                                                                     & vlTOPp->mul_para__DOT__mant_input)) 
                                                                 | ((0x4000U 
                                                                     & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x1ffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0xfU))))) 
                                                                         << 0xeU) 
                                                                        & vlTOPp->mul_para__DOT__mant_input)) 
                                                                    | ((0x2000U 
                                                                        & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x3ffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0xeU))))) 
                                                                            << 0xdU) 
                                                                           & vlTOPp->mul_para__DOT__mant_input)) 
                                                                       | ((0x1000U 
                                                                           & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x7ffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0xdU))))) 
                                                                               << 0xcU) 
                                                                              & vlTOPp->mul_para__DOT__mant_input)) 
                                                                          | ((0x800U 
                                                                              & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0xfffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0xcU))))) 
                                                                                << 0xbU) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                             | ((0x400U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x1fffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0xbU))))) 
                                                                                << 0xaU) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((0x200U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x3fffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 0xaU))))) 
                                                                                << 9U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((0x100U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x7fffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 9U))))) 
                                                                                << 8U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((0x80U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0xffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 8U))))) 
                                                                                << 7U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((0x40U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x1ffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 7U))))) 
                                                                                << 6U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((0x20U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x3ffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 6U))))) 
                                                                                << 5U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((0x10U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x7ffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 5U))))) 
                                                                                << 4U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((8U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0xfffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 4U))))) 
                                                                                << 3U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((4U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x1fffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 3U))))) 
                                                                                << 2U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | ((2U 
                                                                                & (((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x3fffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 2U))))) 
                                                                                << 1U) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)) 
                                                                                | (1U 
                                                                                & ((~ (IData)(
                                                                                (0U 
                                                                                != 
                                                                                (0x7fffffU 
                                                                                & (vlTOPp->mul_para__DOT__mant_input 
                                                                                >> 1U))))) 
                                                                                & vlTOPp->mul_para__DOT__mant_input)))))))))))))))))))))))));
    vlTOPp->mul_para__DOT__u_lod__DOT__z4 = ((0U != 
                                              (0xffffU 
                                               & (vlTOPp->mul_para__DOT__mant_input 
                                                  >> 8U)))
                                              ? vlTOPp->mul_para__DOT__mant_input
                                              : (0xff0000U 
                                                 & (vlTOPp->mul_para__DOT__mant_input 
                                                    << 0x10U)));
    vlTOPp->mul_para__DOT__r_isnan = (((IData)(vlTOPp->mul_para__DOT__a_is_nan) 
                                       | (IData)(vlTOPp->mul_para__DOT__b_is_nan)) 
                                      | (IData)(vlTOPp->mul_para__DOT__r_is0nan));
    vlTOPp->mul_para__DOT__u_lod__DOT__z3 = ((0U != 
                                              (0xffU 
                                               & (vlTOPp->mul_para__DOT__u_lod__DOT__z4 
                                                  >> 0x10U)))
                                              ? vlTOPp->mul_para__DOT__u_lod__DOT__z4
                                              : (0xffff00U 
                                                 & (vlTOPp->mul_para__DOT__u_lod__DOT__z4 
                                                    << 8U)));
    vlTOPp->mul_para__DOT__u_lod__DOT__z2 = ((0U != 
                                              (0xfU 
                                               & (vlTOPp->mul_para__DOT__u_lod__DOT__z2 
                                                  >> 0x14U)))
                                              ? vlTOPp->mul_para__DOT__u_lod__DOT__z3
                                              : (0xfffff0U 
                                                 & (vlTOPp->mul_para__DOT__u_lod__DOT__z3 
                                                    << 4U)));
    vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums 
        = ((0x10U & ((~ (IData)((0U != (0xffffU & (vlTOPp->mul_para__DOT__mant_input 
                                                   >> 8U))))) 
                     << 4U)) | ((8U & ((~ (IData)((0U 
                                                   != 
                                                   (0xffU 
                                                    & (vlTOPp->mul_para__DOT__u_lod__DOT__z4 
                                                       >> 0x10U))))) 
                                       << 3U)) | ((4U 
                                                   & ((~ (IData)(
                                                                 (0U 
                                                                  != 
                                                                  (0xfU 
                                                                   & (vlTOPp->mul_para__DOT__u_lod__DOT__z3 
                                                                      >> 0x14U))))) 
                                                      << 2U)) 
                                                  | ((2U 
                                                      & ((~ (IData)(
                                                                    (0U 
                                                                     != 
                                                                     (3U 
                                                                      & (vlTOPp->mul_para__DOT__u_lod__DOT__z2 
                                                                         >> 0x16U))))) 
                                                         << 1U)) 
                                                     | (1U 
                                                        & (~ 
                                                           (vlTOPp->mul_para__DOT__u_lod__DOT__z1 
                                                            >> 0x17U)))))));
    vlTOPp->mul_para__DOT__zero_nums_c = (0x3fU & (
                                                   (0U 
                                                    != 
                                                    (vlTOPp->mul_para__DOT__mant_1 
                                                     & ((QData)((IData)(vlTOPp->mul_para__DOT__mask_short)) 
                                                        << 0x18U)))
                                                    ? 
                                                   ((IData)(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums) 
                                                    - (IData)(1U))
                                                    : (IData)(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums)));
    if ((1U & ((IData)((vlTOPp->mul_para__DOT__mant_1 
                        >> 0x2fU)) & (~ ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                         >> 9U))))) {
        vlTOPp->mul_para__DOT__expo_2 = (0x3ffU & ((IData)(1U) 
                                                   + (IData)(vlTOPp->mul_para__DOT__expo_1)));
        vlTOPp->mul_para__DOT__mant_2 = (0xffffffffffffULL 
                                         & (0x7fffffffffffULL 
                                            & (vlTOPp->mul_para__DOT__mant_1 
                                               >> 1U)));
    } else {
        vlTOPp->mul_para__DOT__expo_2 = (0x3ffU & (
                                                   (((~ 
                                                      ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                                       >> 9U)) 
                                                     & ((0x1ffU 
                                                         & (IData)(vlTOPp->mul_para__DOT__expo_1)) 
                                                        > (IData)(vlTOPp->mul_para__DOT__zero_nums_c))) 
                                                    & (0U 
                                                       != vlTOPp->mul_para__DOT__mant_1))
                                                    ? 
                                                   ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                                    - (IData)(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums))
                                                    : 0U));
        vlTOPp->mul_para__DOT__mant_2 = (0xffffffffffffULL 
                                         & ((((~ ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                                  >> 9U)) 
                                              & ((0x1ffU 
                                                  & (IData)(vlTOPp->mul_para__DOT__expo_1)) 
                                                 > (IData)(vlTOPp->mul_para__DOT__zero_nums_c))) 
                                             & (0U 
                                                != vlTOPp->mul_para__DOT__mant_1))
                                             ? (vlTOPp->mul_para__DOT__mant_1 
                                                << (IData)(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums))
                                             : (((~ 
                                                  ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                                   >> 9U)) 
                                                 & (0U 
                                                    != 
                                                    (0x1ffU 
                                                     & (IData)(vlTOPp->mul_para__DOT__expo_1))))
                                                 ? 
                                                ((0x2fU 
                                                  >= 
                                                  ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                                   - (IData)(1U)))
                                                  ? 
                                                 (vlTOPp->mul_para__DOT__mant_1 
                                                  << 
                                                  ((IData)(vlTOPp->mul_para__DOT__expo_1) 
                                                   - (IData)(1U)))
                                                  : 0ULL)
                                                 : 
                                                ((0x2fU 
                                                  >= 
                                                  ((IData)(1U) 
                                                   - (IData)(vlTOPp->mul_para__DOT__expo_1)))
                                                  ? 
                                                 (vlTOPp->mul_para__DOT__mant_1 
                                                  >> 
                                                  ((IData)(1U) 
                                                   - (IData)(vlTOPp->mul_para__DOT__expo_1)))
                                                  : 0ULL))));
    }
    vlTOPp->mul_para__DOT__mant_rnd = (0x1ffffffU & 
                                       ((1U & ((3U 
                                                == (IData)(vlTOPp->rnd))
                                                ? (
                                                   (1U 
                                                    & ((IData)(
                                                               (vlTOPp->mul_para__DOT__mant_2 
                                                                >> 0x16U)) 
                                                       & ((IData)(
                                                                  (vlTOPp->mul_para__DOT__mant_2 
                                                                   >> 0x17U)) 
                                                          | ((0U 
                                                              != 
                                                              (0x3fffffU 
                                                               & (IData)(vlTOPp->mul_para__DOT__mant_2))) 
                                                             | (IData)(vlTOPp->mul_para__DOT__bit_s_record)))))
                                                    ? 1U
                                                    : 0U)
                                                : (
                                                   (2U 
                                                    == (IData)(vlTOPp->rnd))
                                                    ? 
                                                   ((IData)(vlTOPp->mul_para__DOT__sign_1)
                                                     ? 0U
                                                     : 1U)
                                                    : 
                                                   ((1U 
                                                     == (IData)(vlTOPp->rnd))
                                                     ? 
                                                    ((IData)(vlTOPp->mul_para__DOT__sign_1)
                                                      ? 1U
                                                      : 0U)
                                                     : 0U))))
                                         ? ((IData)(1U) 
                                            + (IData)(
                                                      (vlTOPp->mul_para__DOT__mant_2 
                                                       >> 0x17U)))
                                         : (IData)(
                                                   (vlTOPp->mul_para__DOT__mant_2 
                                                    >> 0x17U))));
    vlTOPp->mul_para__DOT__expo_3 = (0x3ffU & ((1U 
                                                & ((vlTOPp->mul_para__DOT__mant_rnd 
                                                    >> 0x18U) 
                                                   | ((~ (IData)(
                                                                 (0U 
                                                                  != (IData)(vlTOPp->mul_para__DOT__expo_2)))) 
                                                      & (vlTOPp->mul_para__DOT__mant_rnd 
                                                         >> 0x17U))))
                                                ? ((IData)(1U) 
                                                   + (IData)(vlTOPp->mul_para__DOT__expo_2))
                                                : (IData)(vlTOPp->mul_para__DOT__expo_2)));
    if (((((IData)(vlTOPp->mul_para__DOT__a_is_nan) 
           | (IData)(vlTOPp->mul_para__DOT__b_is_nan)) 
          | (IData)(vlTOPp->mul_para__DOT__a_is_inf)) 
         | (IData)(vlTOPp->mul_para__DOT__b_is_inf))) {
        vlTOPp->mul_para__DOT__res_sign = (1U & ((IData)(vlTOPp->mul_para__DOT__r_isnan)
                                                  ? 
                                                 (((0xffU 
                                                    & (vlTOPp->a 
                                                       >> 0x17U)) 
                                                   > 
                                                   (0xffU 
                                                    & (vlTOPp->b 
                                                       >> 0x17U)))
                                                   ? 
                                                  ((vlTOPp->a 
                                                    >> 0x1fU) 
                                                   | (IData)(vlTOPp->mul_para__DOT__r_is0nan))
                                                   : 
                                                  ((vlTOPp->b 
                                                    >> 0x1fU) 
                                                   | (IData)(vlTOPp->mul_para__DOT__r_is0nan)))
                                                  : 
                                                 ((IData)(vlTOPp->mul_para__DOT__r_is0nan) 
                                                  | (IData)(vlTOPp->mul_para__DOT__sign_1))));
        vlTOPp->mul_para__DOT__res_expo = 0xffU;
        vlTOPp->mul_para__DOT__res_mant = ((IData)(vlTOPp->mul_para__DOT__r_isnan)
                                            ? (((0xffU 
                                                 & (vlTOPp->a 
                                                    >> 0x17U)) 
                                                > (0xffU 
                                                   & (vlTOPp->b 
                                                      >> 0x17U)))
                                                ? (0x400000U 
                                                   | (0x3fffffU 
                                                      & vlTOPp->a))
                                                : (0x400000U 
                                                   | (0x3fffffU 
                                                      & vlTOPp->b)))
                                            : 0U);
    } else {
        vlTOPp->mul_para__DOT__res_sign = vlTOPp->mul_para__DOT__sign_1;
        if (((0xffU <= (IData)(vlTOPp->mul_para__DOT__expo_2)) 
             | (0xffU <= (IData)(vlTOPp->mul_para__DOT__expo_3)))) {
            if ((((2U == (IData)(vlTOPp->rnd)) | (0U 
                                                  == (IData)(vlTOPp->rnd))) 
                 & (IData)(vlTOPp->mul_para__DOT__res_sign))) {
                vlTOPp->mul_para__DOT__res_expo = 0xffU;
                vlTOPp->mul_para__DOT__res_mant = 0U;
            } else {
                vlTOPp->mul_para__DOT__res_expo = 0xfeU;
                vlTOPp->mul_para__DOT__res_mant = 0x7fffffU;
            }
        } else {
            vlTOPp->mul_para__DOT__res_expo = (0xffU 
                                               & (IData)(vlTOPp->mul_para__DOT__expo_3));
            vlTOPp->mul_para__DOT__res_mant = (0x7fffffU 
                                               & ((0x1000000U 
                                                   & vlTOPp->mul_para__DOT__mant_rnd)
                                                   ? 
                                                  (vlTOPp->mul_para__DOT__mant_rnd 
                                                   >> 1U)
                                                   : vlTOPp->mul_para__DOT__mant_rnd));
        }
    }
    vlTOPp->res = (((IData)(vlTOPp->mul_para__DOT__res_sign) 
                    << 0x1fU) | (((IData)(vlTOPp->mul_para__DOT__res_expo) 
                                  << 0x17U) | vlTOPp->mul_para__DOT__res_mant));
}

void Vmul_para::_eval(Vmul_para__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_eval\n"); );
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
    vlTOPp->__Vm_traceActivity[1U] = 1U;
}

VL_INLINE_OPT QData Vmul_para::_change_request(Vmul_para__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_change_request\n"); );
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData Vmul_para::_change_request_1(Vmul_para__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_change_request_1\n"); );
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    __req |= ((vlTOPp->mul_para__DOT__u_lod__DOT__z2 ^ vlTOPp->__Vchglast__TOP__mul_para__DOT__u_lod__DOT__z2)
         | (vlTOPp->mul_para__DOT__u_lod__DOT__z1 ^ vlTOPp->__Vchglast__TOP__mul_para__DOT__u_lod__DOT__z1));
    VL_DEBUG_IF( if(__req && ((vlTOPp->mul_para__DOT__u_lod__DOT__z2 ^ vlTOPp->__Vchglast__TOP__mul_para__DOT__u_lod__DOT__z2))) VL_DBG_MSGF("        CHANGE: mul_lod.sv:30: mul_para.u_lod.z2\n"); );
    VL_DEBUG_IF( if(__req && ((vlTOPp->mul_para__DOT__u_lod__DOT__z1 ^ vlTOPp->__Vchglast__TOP__mul_para__DOT__u_lod__DOT__z1))) VL_DBG_MSGF("        CHANGE: mul_lod.sv:31: mul_para.u_lod.z1\n"); );
    // Final
    vlTOPp->__Vchglast__TOP__mul_para__DOT__u_lod__DOT__z2 
        = vlTOPp->mul_para__DOT__u_lod__DOT__z2;
    vlTOPp->__Vchglast__TOP__mul_para__DOT__u_lod__DOT__z1 
        = vlTOPp->mul_para__DOT__u_lod__DOT__z1;
    return __req;
}

#ifdef VL_DEBUG
void Vmul_para::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmul_para::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((rnd & 0xfcU))) {
        Verilated::overWidthError("rnd");}
}
#endif  // VL_DEBUG
