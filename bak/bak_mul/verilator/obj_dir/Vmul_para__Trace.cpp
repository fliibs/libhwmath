// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vmul_para__Syms.h"


void Vmul_para::traceChgTop0(void* userp, VerilatedVcd* tracep) {
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    {
        vlTOPp->traceChgSub0(userp, tracep);
    }
}

void Vmul_para::traceChgSub0(void* userp, VerilatedVcd* tracep) {
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode + 1);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[1U])) {
            tracep->chgBit(oldp+0,(vlTOPp->mul_para__DOT__a_is_nan));
            tracep->chgBit(oldp+1,(vlTOPp->mul_para__DOT__b_is_nan));
            tracep->chgBit(oldp+2,(vlTOPp->mul_para__DOT__a_is_inf));
            tracep->chgBit(oldp+3,(vlTOPp->mul_para__DOT__b_is_inf));
            tracep->chgBit(oldp+4,(vlTOPp->mul_para__DOT__res_sign));
            tracep->chgCData(oldp+5,(vlTOPp->mul_para__DOT__res_expo),8);
            tracep->chgIData(oldp+6,(vlTOPp->mul_para__DOT__res_mant),23);
            tracep->chgBit(oldp+7,(vlTOPp->mul_para__DOT__sign_1));
            tracep->chgSData(oldp+8,(vlTOPp->mul_para__DOT__expo_1),10);
            tracep->chgQData(oldp+9,(vlTOPp->mul_para__DOT__mant_1),48);
            tracep->chgIData(oldp+11,(vlTOPp->mul_para__DOT__mant_input),24);
            tracep->chgIData(oldp+12,(vlTOPp->mul_para__DOT__mask_short),24);
            tracep->chgQData(oldp+13,(((QData)((IData)(vlTOPp->mul_para__DOT__mask_short)) 
                                       << 0x18U)),48);
            tracep->chgBit(oldp+15,((0U != (vlTOPp->mul_para__DOT__mant_1 
                                            & ((QData)((IData)(vlTOPp->mul_para__DOT__mask_short)) 
                                               << 0x18U)))));
            tracep->chgCData(oldp+16,(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums),6);
            tracep->chgCData(oldp+17,(vlTOPp->mul_para__DOT__zero_nums_c),6);
            tracep->chgSData(oldp+18,(vlTOPp->mul_para__DOT__expo_2),10);
            tracep->chgQData(oldp+19,(vlTOPp->mul_para__DOT__mant_2),48);
            tracep->chgBit(oldp+21,(vlTOPp->mul_para__DOT__bit_s_record));
            tracep->chgBit(oldp+22,((1U & (IData)((vlTOPp->mul_para__DOT__mant_2 
                                                   >> 0x17U)))));
            tracep->chgBit(oldp+23,((1U & (IData)((vlTOPp->mul_para__DOT__mant_2 
                                                   >> 0x16U)))));
            tracep->chgBit(oldp+24,(((0U != (0x3fffffU 
                                             & (IData)(vlTOPp->mul_para__DOT__mant_2))) 
                                     | (IData)(vlTOPp->mul_para__DOT__bit_s_record))));
            tracep->chgIData(oldp+25,((0x1ffffffU & 
                                       ((IData)(1U) 
                                        + (IData)((vlTOPp->mul_para__DOT__mant_2 
                                                   >> 0x17U))))),25);
            tracep->chgSData(oldp+26,((0x3ffU & ((IData)(1U) 
                                                 + (IData)(vlTOPp->mul_para__DOT__expo_2)))),10);
            tracep->chgIData(oldp+27,(vlTOPp->mul_para__DOT__mant_rnd),25);
            tracep->chgSData(oldp+28,(vlTOPp->mul_para__DOT__expo_3),10);
            tracep->chgIData(oldp+29,((0x7fffffU & 
                                       ((0x1000000U 
                                         & vlTOPp->mul_para__DOT__mant_rnd)
                                         ? (vlTOPp->mul_para__DOT__mant_rnd 
                                            >> 1U) : vlTOPp->mul_para__DOT__mant_rnd))),23);
            tracep->chgBit(oldp+30,(((0xffU <= (IData)(vlTOPp->mul_para__DOT__expo_2)) 
                                     | (0xffU <= (IData)(vlTOPp->mul_para__DOT__expo_3)))));
            tracep->chgBit(oldp+31,(vlTOPp->mul_para__DOT__r_isnan));
            tracep->chgBit(oldp+32,(((((IData)(vlTOPp->mul_para__DOT__a_is_nan) 
                                       | (IData)(vlTOPp->mul_para__DOT__b_is_nan)) 
                                      | (IData)(vlTOPp->mul_para__DOT__a_is_inf)) 
                                     | (IData)(vlTOPp->mul_para__DOT__b_is_inf))));
            tracep->chgBit(oldp+33,(vlTOPp->mul_para__DOT__r_is0nan));
            tracep->chgCData(oldp+34,(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums),5);
            tracep->chgIData(oldp+35,(vlTOPp->mul_para__DOT__u_lod__DOT__z4),24);
            tracep->chgIData(oldp+36,(vlTOPp->mul_para__DOT__u_lod__DOT__z3),24);
            tracep->chgIData(oldp+37,(vlTOPp->mul_para__DOT__u_lod__DOT__z2),24);
            tracep->chgIData(oldp+38,(vlTOPp->mul_para__DOT__u_lod__DOT__z1),24);
        }
        tracep->chgIData(oldp+39,(vlTOPp->a),32);
        tracep->chgIData(oldp+40,(vlTOPp->b),32);
        tracep->chgCData(oldp+41,(vlTOPp->rnd),2);
        tracep->chgIData(oldp+42,(vlTOPp->res),32);
        tracep->chgBit(oldp+43,((0U != (0xffU & (vlTOPp->a 
                                                 >> 0x17U)))));
        tracep->chgBit(oldp+44,((0U != (0xffU & (vlTOPp->b 
                                                 >> 0x17U)))));
        tracep->chgBit(oldp+45,((1U & (vlTOPp->a >> 0x1fU))));
        tracep->chgCData(oldp+46,((0xffU & (vlTOPp->a 
                                            >> 0x17U))),8);
        tracep->chgIData(oldp+47,((0x7fffffU & vlTOPp->a)),23);
        tracep->chgBit(oldp+48,((1U & (vlTOPp->b >> 0x1fU))));
        tracep->chgCData(oldp+49,((0xffU & (vlTOPp->b 
                                            >> 0x17U))),8);
        tracep->chgIData(oldp+50,((0x7fffffU & vlTOPp->b)),23);
        tracep->chgBit(oldp+51,((1U & ((3U == (IData)(vlTOPp->rnd))
                                        ? ((1U & ((IData)(
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
                                            ? 1U : 0U)
                                        : ((2U == (IData)(vlTOPp->rnd))
                                            ? ((IData)(vlTOPp->mul_para__DOT__sign_1)
                                                ? 0U
                                                : 1U)
                                            : ((1U 
                                                == (IData)(vlTOPp->rnd))
                                                ? ((IData)(vlTOPp->mul_para__DOT__sign_1)
                                                    ? 1U
                                                    : 0U)
                                                : 0U))))));
        tracep->chgBit(oldp+52,((1U & ((IData)(vlTOPp->mul_para__DOT__r_isnan)
                                        ? (((0xffU 
                                             & (vlTOPp->a 
                                                >> 0x17U)) 
                                            > (0xffU 
                                               & (vlTOPp->b 
                                                  >> 0x17U)))
                                            ? ((vlTOPp->a 
                                                >> 0x1fU) 
                                               | (IData)(vlTOPp->mul_para__DOT__r_is0nan))
                                            : ((vlTOPp->b 
                                                >> 0x1fU) 
                                               | (IData)(vlTOPp->mul_para__DOT__r_is0nan)))
                                        : ((IData)(vlTOPp->mul_para__DOT__r_is0nan) 
                                           | (IData)(vlTOPp->mul_para__DOT__sign_1))))));
        tracep->chgIData(oldp+53,(((IData)(vlTOPp->mul_para__DOT__r_isnan)
                                    ? (((0xffU & (vlTOPp->a 
                                                  >> 0x17U)) 
                                        > (0xffU & 
                                           (vlTOPp->b 
                                            >> 0x17U)))
                                        ? (0x400000U 
                                           | (0x3fffffU 
                                              & vlTOPp->a))
                                        : (0x400000U 
                                           | (0x3fffffU 
                                              & vlTOPp->b)))
                                    : 0U)),23);
    }
}

void Vmul_para::traceCleanup(void* userp, VerilatedVcd* /*unused*/) {
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlSymsp->__Vm_activity = false;
        vlTOPp->__Vm_traceActivity[0U] = 0U;
        vlTOPp->__Vm_traceActivity[1U] = 0U;
    }
}
