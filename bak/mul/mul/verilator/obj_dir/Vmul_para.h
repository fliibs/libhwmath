// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VMUL_PARA_H_
#define _VMUL_PARA_H_  // guard

#include "verilated.h"

//==========

class Vmul_para__Syms;
class Vmul_para_VerilatedVcd;


//----------

VL_MODULE(Vmul_para) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(rnd,1,0);
    VL_IN(a,31,0);
    VL_IN(b,31,0);
    VL_OUT(res,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*0:0*/ mul_para__DOT__a_is_nan;
    CData/*0:0*/ mul_para__DOT__b_is_nan;
    CData/*0:0*/ mul_para__DOT__a_is_inf;
    CData/*0:0*/ mul_para__DOT__b_is_inf;
    CData/*0:0*/ mul_para__DOT__res_sign;
    CData/*7:0*/ mul_para__DOT__res_expo;
    CData/*0:0*/ mul_para__DOT__sign_1;
    CData/*5:0*/ mul_para__DOT__zero_nums_c;
    CData/*0:0*/ mul_para__DOT__bit_s_record;
    CData/*0:0*/ mul_para__DOT__r_isnan;
    CData/*0:0*/ mul_para__DOT__r_is0nan;
    SData/*9:0*/ mul_para__DOT__expo_1;
    SData/*9:0*/ mul_para__DOT__expo_2;
    SData/*9:0*/ mul_para__DOT__expo_rnd;
    SData/*9:0*/ mul_para__DOT__expo_3;
    IData/*22:0*/ mul_para__DOT__res_mant;
    IData/*23:0*/ mul_para__DOT__mant_input;
    IData/*23:0*/ mul_para__DOT__mask_short;
    IData/*24:0*/ mul_para__DOT__mant_rnd;
    IData/*23:0*/ mul_para__DOT__u_lod__DOT__z4;
    IData/*23:0*/ mul_para__DOT__u_lod__DOT__z3;
    IData/*23:0*/ mul_para__DOT__u_lod__DOT__z2;
    IData/*23:0*/ mul_para__DOT__u_lod__DOT__z1;
    IData/*23:0*/ mul_para__DOT__u_lod__DOT__z0;
    QData/*47:0*/ mul_para__DOT__mant_1;
    QData/*47:0*/ mul_para__DOT__mant_2;
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*4:0*/ mul_para__DOT____Vcellout__u_lod__zero_nums;
    IData/*23:0*/ __Vchglast__TOP__mul_para__DOT__u_lod__DOT__z2;
    IData/*23:0*/ __Vchglast__TOP__mul_para__DOT__u_lod__DOT__z1;
    CData/*0:0*/ __Vm_traceActivity[2];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vmul_para__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vmul_para);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    Vmul_para(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vmul_para();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vmul_para__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vmul_para__Syms* symsp, bool first);
  private:
    static QData _change_request(Vmul_para__Syms* __restrict vlSymsp);
    static QData _change_request_1(Vmul_para__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__1(Vmul_para__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vmul_para__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vmul_para__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vmul_para__Syms* __restrict vlSymsp) VL_ATTR_COLD;
  private:
    static void traceChgSub0(void* userp, VerilatedVcd* tracep);
    static void traceChgTop0(void* userp, VerilatedVcd* tracep);
    static void traceCleanup(void* userp, VerilatedVcd* /*unused*/);
    static void traceFullSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceFullTop0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitTop(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    void traceRegister(VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
