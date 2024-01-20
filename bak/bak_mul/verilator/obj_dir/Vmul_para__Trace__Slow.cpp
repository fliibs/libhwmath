// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vmul_para__Syms.h"


//======================

void Vmul_para::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addInitCb(&traceInit, __VlSymsp);
    traceRegister(tfp->spTrace());
}

void Vmul_para::traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->module(vlSymsp->name());
    tracep->scopeEscape(' ');
    Vmul_para::traceInitTop(vlSymsp, tracep);
    tracep->scopeEscape('.');
}

//======================


void Vmul_para::traceInitTop(void* userp, VerilatedVcd* tracep) {
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceInitSub0(userp, tracep);
    }
}

void Vmul_para::traceInitSub0(void* userp, VerilatedVcd* tracep) {
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    const int c = vlSymsp->__Vm_baseCode;
    if (false && tracep && c) {}  // Prevent unused
    // Body
    {
        tracep->declBus(c+40,"a", false,-1, 31,0);
        tracep->declBus(c+41,"b", false,-1, 31,0);
        tracep->declBus(c+42,"rnd", false,-1, 1,0);
        tracep->declBus(c+43,"res", false,-1, 31,0);
        tracep->declBus(c+55,"mul_para SIGN_W", false,-1, 31,0);
        tracep->declBus(c+56,"mul_para EXPO_W", false,-1, 31,0);
        tracep->declBus(c+57,"mul_para MANT_W", false,-1, 31,0);
        tracep->declBus(c+40,"mul_para a", false,-1, 31,0);
        tracep->declBus(c+41,"mul_para b", false,-1, 31,0);
        tracep->declBus(c+42,"mul_para rnd", false,-1, 1,0);
        tracep->declBus(c+43,"mul_para res", false,-1, 31,0);
        tracep->declBit(c+44,"mul_para a_is_n0", false,-1);
        tracep->declBit(c+45,"mul_para b_is_n0", false,-1);
        tracep->declBit(c+1,"mul_para a_is_nan", false,-1);
        tracep->declBit(c+2,"mul_para b_is_nan", false,-1);
        tracep->declBit(c+3,"mul_para a_is_inf", false,-1);
        tracep->declBit(c+4,"mul_para b_is_inf", false,-1);
        tracep->declBit(c+46,"mul_para a_sign", false,-1);
        tracep->declBus(c+47,"mul_para a_expo", false,-1, 7,0);
        tracep->declBus(c+48,"mul_para a_mant", false,-1, 22,0);
        tracep->declBit(c+49,"mul_para b_sign", false,-1);
        tracep->declBus(c+50,"mul_para b_expo", false,-1, 7,0);
        tracep->declBus(c+51,"mul_para b_mant", false,-1, 22,0);
        tracep->declBit(c+5,"mul_para res_sign", false,-1);
        tracep->declBus(c+6,"mul_para res_expo", false,-1, 7,0);
        tracep->declBus(c+7,"mul_para res_mant", false,-1, 22,0);
        tracep->declBit(c+8,"mul_para sign_1", false,-1);
        tracep->declBus(c+9,"mul_para expo_1", false,-1, 9,0);
        tracep->declQuad(c+10,"mul_para mant_1", false,-1, 47,0);
        tracep->declBus(c+12,"mul_para mant_input", false,-1, 23,0);
        tracep->declBus(c+13,"mul_para mask_short", false,-1, 23,0);
        tracep->declQuad(c+14,"mul_para mask_long", false,-1, 47,0);
        tracep->declBit(c+16,"mul_para revise", false,-1);
        tracep->declBus(c+17,"mul_para zero_nums_uc", false,-1, 5,0);
        tracep->declBus(c+18,"mul_para zero_nums_c", false,-1, 5,0);
        tracep->declBus(c+19,"mul_para expo_2", false,-1, 9,0);
        tracep->declQuad(c+20,"mul_para mant_2", false,-1, 47,0);
        tracep->declBit(c+22,"mul_para bit_s_record", false,-1);
        tracep->declBit(c+23,"mul_para g", false,-1);
        tracep->declBit(c+24,"mul_para r", false,-1);
        tracep->declBit(c+25,"mul_para s", false,-1);
        tracep->declBit(c+52,"mul_para carry", false,-1);
        tracep->declBus(c+26,"mul_para mant_inr_1", false,-1, 24,0);
        tracep->declBus(c+27,"mul_para expo_inr_1", false,-1, 9,0);
        tracep->declBus(c+28,"mul_para mant_rnd", false,-1, 24,0);
        tracep->declBus(c+58,"mul_para expo_rnd", false,-1, 9,0);
        tracep->declBus(c+29,"mul_para expo_3", false,-1, 9,0);
        tracep->declBus(c+30,"mul_para mant_3", false,-1, 22,0);
        tracep->declBit(c+31,"mul_para overflow", false,-1);
        tracep->declBit(c+32,"mul_para r_isnan", false,-1);
        tracep->declBit(c+33,"mul_para is_inf_nan", false,-1);
        tracep->declBit(c+34,"mul_para r_is0nan", false,-1);
        tracep->declBit(c+53,"mul_para sign_nan", false,-1);
        tracep->declBus(c+54,"mul_para mant_4", false,-1, 22,0);
        tracep->declBus(c+59,"mul_para u_lod INPUT_WIDTH", false,-1, 31,0);
        tracep->declBus(c+60,"mul_para u_lod AWIDTH", false,-1, 31,0);
        tracep->declBus(c+12,"mul_para u_lod in_detect", false,-1, 23,0);
        tracep->declBus(c+35,"mul_para u_lod zero_nums", false,-1, 4,0);
        tracep->declBus(c+36,"mul_para u_lod z4", false,-1, 23,0);
        tracep->declBus(c+37,"mul_para u_lod z3", false,-1, 23,0);
        tracep->declBus(c+38,"mul_para u_lod z2", false,-1, 23,0);
        tracep->declBus(c+39,"mul_para u_lod z1", false,-1, 23,0);
        tracep->declBus(c+61,"mul_para u_lod z0", false,-1, 23,0);
        tracep->declBus(c+59,"mul_para u_mask INPUT_WIDTH", false,-1, 31,0);
        tracep->declBus(c+12,"mul_para u_mask input_unmask", false,-1, 23,0);
        tracep->declBus(c+13,"mul_para u_mask output_mask", false,-1, 23,0);
    }
}

void Vmul_para::traceRegister(VerilatedVcd* tracep) {
    // Body
    {
        tracep->addFullCb(&traceFullTop0, __VlSymsp);
        tracep->addChgCb(&traceChgTop0, __VlSymsp);
        tracep->addCleanupCb(&traceCleanup, __VlSymsp);
    }
}

void Vmul_para::traceFullTop0(void* userp, VerilatedVcd* tracep) {
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceFullSub0(userp, tracep);
    }
}

void Vmul_para::traceFullSub0(void* userp, VerilatedVcd* tracep) {
    Vmul_para__Syms* __restrict vlSymsp = static_cast<Vmul_para__Syms*>(userp);
    Vmul_para* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        tracep->fullBit(oldp+1,(vlTOPp->mul_para__DOT__a_is_nan));
        tracep->fullBit(oldp+2,(vlTOPp->mul_para__DOT__b_is_nan));
        tracep->fullBit(oldp+3,(vlTOPp->mul_para__DOT__a_is_inf));
        tracep->fullBit(oldp+4,(vlTOPp->mul_para__DOT__b_is_inf));
        tracep->fullBit(oldp+5,(vlTOPp->mul_para__DOT__res_sign));
        tracep->fullCData(oldp+6,(vlTOPp->mul_para__DOT__res_expo),8);
        tracep->fullIData(oldp+7,(vlTOPp->mul_para__DOT__res_mant),23);
        tracep->fullBit(oldp+8,(vlTOPp->mul_para__DOT__sign_1));
        tracep->fullSData(oldp+9,(vlTOPp->mul_para__DOT__expo_1),10);
        tracep->fullQData(oldp+10,(vlTOPp->mul_para__DOT__mant_1),48);
        tracep->fullIData(oldp+12,(vlTOPp->mul_para__DOT__mant_input),24);
        tracep->fullIData(oldp+13,(vlTOPp->mul_para__DOT__mask_short),24);
        tracep->fullQData(oldp+14,(((QData)((IData)(vlTOPp->mul_para__DOT__mask_short)) 
                                    << 0x18U)),48);
        tracep->fullBit(oldp+16,((0U != (vlTOPp->mul_para__DOT__mant_1 
                                         & ((QData)((IData)(vlTOPp->mul_para__DOT__mask_short)) 
                                            << 0x18U)))));
        tracep->fullCData(oldp+17,(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums),6);
        tracep->fullCData(oldp+18,(vlTOPp->mul_para__DOT__zero_nums_c),6);
        tracep->fullSData(oldp+19,(vlTOPp->mul_para__DOT__expo_2),10);
        tracep->fullQData(oldp+20,(vlTOPp->mul_para__DOT__mant_2),48);
        tracep->fullBit(oldp+22,(vlTOPp->mul_para__DOT__bit_s_record));
        tracep->fullBit(oldp+23,((1U & (IData)((vlTOPp->mul_para__DOT__mant_2 
                                                >> 0x17U)))));
        tracep->fullBit(oldp+24,((1U & (IData)((vlTOPp->mul_para__DOT__mant_2 
                                                >> 0x16U)))));
        tracep->fullBit(oldp+25,(((0U != (0x3fffffU 
                                          & (IData)(vlTOPp->mul_para__DOT__mant_2))) 
                                  | (IData)(vlTOPp->mul_para__DOT__bit_s_record))));
        tracep->fullIData(oldp+26,((0x1ffffffU & ((IData)(1U) 
                                                  + (IData)(
                                                            (vlTOPp->mul_para__DOT__mant_2 
                                                             >> 0x17U))))),25);
        tracep->fullSData(oldp+27,((0x3ffU & ((IData)(1U) 
                                              + (IData)(vlTOPp->mul_para__DOT__expo_2)))),10);
        tracep->fullIData(oldp+28,(vlTOPp->mul_para__DOT__mant_rnd),25);
        tracep->fullSData(oldp+29,(vlTOPp->mul_para__DOT__expo_3),10);
        tracep->fullIData(oldp+30,((0x7fffffU & ((0x1000000U 
                                                  & vlTOPp->mul_para__DOT__mant_rnd)
                                                  ? 
                                                 (vlTOPp->mul_para__DOT__mant_rnd 
                                                  >> 1U)
                                                  : vlTOPp->mul_para__DOT__mant_rnd))),23);
        tracep->fullBit(oldp+31,(((0xffU <= (IData)(vlTOPp->mul_para__DOT__expo_2)) 
                                  | (0xffU <= (IData)(vlTOPp->mul_para__DOT__expo_3)))));
        tracep->fullBit(oldp+32,(vlTOPp->mul_para__DOT__r_isnan));
        tracep->fullBit(oldp+33,(((((IData)(vlTOPp->mul_para__DOT__a_is_nan) 
                                    | (IData)(vlTOPp->mul_para__DOT__b_is_nan)) 
                                   | (IData)(vlTOPp->mul_para__DOT__a_is_inf)) 
                                  | (IData)(vlTOPp->mul_para__DOT__b_is_inf))));
        tracep->fullBit(oldp+34,(vlTOPp->mul_para__DOT__r_is0nan));
        tracep->fullCData(oldp+35,(vlTOPp->mul_para__DOT____Vcellout__u_lod__zero_nums),5);
        tracep->fullIData(oldp+36,(vlTOPp->mul_para__DOT__u_lod__DOT__z4),24);
        tracep->fullIData(oldp+37,(vlTOPp->mul_para__DOT__u_lod__DOT__z3),24);
        tracep->fullIData(oldp+38,(vlTOPp->mul_para__DOT__u_lod__DOT__z2),24);
        tracep->fullIData(oldp+39,(vlTOPp->mul_para__DOT__u_lod__DOT__z1),24);
        tracep->fullIData(oldp+40,(vlTOPp->a),32);
        tracep->fullIData(oldp+41,(vlTOPp->b),32);
        tracep->fullCData(oldp+42,(vlTOPp->rnd),2);
        tracep->fullIData(oldp+43,(vlTOPp->res),32);
        tracep->fullBit(oldp+44,((0U != (0xffU & (vlTOPp->a 
                                                  >> 0x17U)))));
        tracep->fullBit(oldp+45,((0U != (0xffU & (vlTOPp->b 
                                                  >> 0x17U)))));
        tracep->fullBit(oldp+46,((1U & (vlTOPp->a >> 0x1fU))));
        tracep->fullCData(oldp+47,((0xffU & (vlTOPp->a 
                                             >> 0x17U))),8);
        tracep->fullIData(oldp+48,((0x7fffffU & vlTOPp->a)),23);
        tracep->fullBit(oldp+49,((1U & (vlTOPp->b >> 0x1fU))));
        tracep->fullCData(oldp+50,((0xffU & (vlTOPp->b 
                                             >> 0x17U))),8);
        tracep->fullIData(oldp+51,((0x7fffffU & vlTOPp->b)),23);
        tracep->fullBit(oldp+52,((1U & ((3U == (IData)(vlTOPp->rnd))
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
                                                 ? 
                                                ((IData)(vlTOPp->mul_para__DOT__sign_1)
                                                  ? 1U
                                                  : 0U)
                                                 : 0U))))));
        tracep->fullBit(oldp+53,((1U & ((IData)(vlTOPp->mul_para__DOT__r_isnan)
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
        tracep->fullIData(oldp+54,(((IData)(vlTOPp->mul_para__DOT__r_isnan)
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
        tracep->fullIData(oldp+55,(1U),32);
        tracep->fullIData(oldp+56,(8U),32);
        tracep->fullIData(oldp+57,(0x17U),32);
        tracep->fullSData(oldp+58,(vlTOPp->mul_para__DOT__expo_rnd),10);
        tracep->fullIData(oldp+59,(0x18U),32);
        tracep->fullIData(oldp+60,(4U),32);
        tracep->fullIData(oldp+61,(vlTOPp->mul_para__DOT__u_lod__DOT__z0),24);
    }
}
