//******************************************************************
// datapath.h - datapath
// Author: Nick Birch
// Date: July 31, 2016
//******************************************************************

#ifndef DATAPATH_H
#define DATAPATH_H
#include "systemc.h"
#include "modules.h"

// datapath module
SC_MODULE(datapath) {
    // ports
    
    sc_in<sc_logic> HI_mux_sel, LO_mux_sel, HI_mux2_sel, carry_mux_sel; 
    sc_in<sc_logic> HI_reg_load, LO_reg_load, rshift_load;
	sc_out< sc_uint<NN_HALF_DIGIT_BITS> > LO_LSB, HI_LSB;
    
    sc_in< sc_uint<NN_HALF_DIGIT_BITS> > multiplier, multiplicand, HI_mux_zero, carry_mux_zero;
    sc_out< sc_uint<NN_DIGIT_BITS> > product;
	
    sc_in<sc_logic> reset;
    sc_in_clk clock;

    // signals
    sc_signal< sc_uint<NN_HALF_DIGIT_BITS> > HI_mux_out, LO_mux_out;
    sc_signal< sc_uint<NN_HALF_DIGIT_BITS> > HI_reg_out, LO_reg_out;
    sc_signal< sc_uint<NN_HALF_DIGIT_BITS> > adder_out, adder_carry_out, HI_mux2_out, carry_mux_out, HI_out, LO_out;
    sc_signal< sc_uint<NN_DIGIT_BITS> > rshift_out;
        
    // instances   
    mux2 HI_mux, LO_mux, HI_mux2, carry_out_mux; 
    reg HI_reg, LO_reg;
    add adder;
    rshift rshifter;
    splitter splitty;
    
    SC_CTOR(datapath): 
	HI_mux("HI_mux"), LO_mux("LO_mux"), HI_mux2("HI_mux2"), carry_out_mux("carry_out_mux"),
    HI_reg("HI_reg"), LO_reg("LO_reg"),
	adder("adder"), rshifter("rshifter"),
	splitty("splitty") { 
	   
       // Interconnections
	                  
        HI_mux.A_IN(HI_reg_out);
        HI_mux.B_IN(HI_mux_zero);
        HI_mux.Z_OUT(HI_mux_out);
        HI_mux.sel(HI_mux_sel);
            
        LO_mux.A_IN(LO_out);
        LO_mux.B_IN(multiplier);
        LO_mux.Z_OUT(LO_mux_out);
        LO_mux.sel(LO_mux_sel);
            
        HI_mux2.A_IN(adder_out);
        HI_mux2.B_IN(HI_reg_out);
        HI_mux2.Z_OUT(HI_mux2_out);
        HI_mux2.sel(HI_mux2_sel);
            
        carry_out_mux.A_IN(adder_carry_out);
        carry_out_mux.B_IN(carry_mux_zero);
        carry_out_mux.Z_OUT(carry_mux_out);
        carry_out_mux.sel(carry_mux_sel);
        
        HI_reg.clock(clock);
        HI_reg.reset(reset);
        HI_reg.load(HI_reg_load);
        HI_reg.A_IN(HI_out);
        HI_reg.Z_OUT(HI_reg_out);
        HI_reg.LSB(HI_LSB);
            
        LO_reg.clock(clock);
        LO_reg.reset(reset);
        LO_reg.load(LO_reg_load);
        LO_reg.A_IN(LO_mux_out);
        LO_reg.Z_OUT(LO_reg_out);
        LO_reg.LSB(LO_LSB);

        adder.A_IN(HI_mux_out);
        adder.B_IN(multiplicand);
        adder.carry_OUT(adder_carry_out);
        adder.Z_OUT(adder_out);
	
        rshifter.reset(reset);
        rshifter.clock(clock);
        rshifter.A_IN(HI_mux2_out);
        rshifter.B_IN(LO_reg_out);
        rshifter.load(rshift_load);
        rshifter.carry_IN(carry_mux_out);
        rshifter.Z_OUT(rshift_out);
        rshifter.product_OUT(product);


        splitty.A_IN(rshift_out);
        splitty.LO_OUT(LO_out);
        splitty.HI_OUT(HI_out);
		
    }
};

#endif