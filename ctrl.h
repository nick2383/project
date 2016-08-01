//******************************************************************
// ctrl.h - header file for datapath controller
// Author: Nick Birch
// Date: July 31, 2016
//******************************************************************

#ifndef CONTROL_H
#define CONTROL_H
#include "systemc.h"
#include "digit.h"

// #define input_length 4
// #define product_length 8

// #define input_length 16
// #define product_length 32

enum mult_ctrl_state {S0_BEGIN, S1_COUNT, S2_CHECK, S3_DO_NOTHING, S3_ADD, S4_SHIFT, S5_WAIT, S6_FINISH};
    
//*************************
// controller module
//*************************
SC_MODULE(ctrl) {
    // declare ports
    sc_out<sc_logic> HI_mux_sel, LO_mux_sel, HI_mux2_sel, carry_mux_sel;
    sc_out<sc_logic> HI_reg_load, LO_reg_load, rshift_load;

    sc_in<sc_logic> START;
    sc_out<sc_logic> FINISH;
    sc_in<sc_uint<NN_HALF_DIGIT_BITS> > LO_LSB;

    sc_in<sc_logic> reset;
    sc_in_clk clock;
    
    // signals
    sc_signal<mult_ctrl_state> state, next_state;

    // variables
    int counter;
    
    // functions   
    void state_transition();
    void state_output();
    void state_reg();
   
    SC_CTOR(ctrl) {
        SC_METHOD(state_transition);
    	   sensitive << state;
    	SC_METHOD(state_output);
    	   sensitive << state;	
    	SC_CTHREAD(state_reg, clock.pos());
    }
};

#endif