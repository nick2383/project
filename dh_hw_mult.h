#include "systemc.h"
#include "digit.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

// enumerated types for states
enum ctrl_state {S0_WAIT, S1_EXECUTE, S2_OUTPUT, S3_FINISH};

SC_MODULE (dh_hw_mult)
{
  sc_in<bool> hw_mult_enable; 
  sc_in<NN_DIGIT> in_data_1;
  sc_in<NN_DIGIT> in_data_2;
  sc_out<NN_DIGIT> out_data_low;
  sc_out<NN_DIGIT> out_data_high;
  sc_out<bool> hw_mult_done;

  // void process_hw_mult();

  //*************
  // Added Ports
  //*************
  sc_in_clk clock;   // clock input

  //**************
  // Added Signals
  //**************
  sc_in<sc_logic> reset; // reset signal
  sc_signal<ctrl_state> state, next_state;  // state machine signals

  //**************
  // Added Functions
  //**************
  void state_transition();
  void state_output();
  void state_reg();

  
  SC_CTOR (dh_hw_mult)
  {
    //SC_THREAD (process_hw_mult);
    //sensitive << hw_mult_enable;

    SC_METHOD(state_transition);
      sensitive << state << hw_mult_enable;
    SC_METHOD(state_output);
      sensitive << state; 
    SC_CTHREAD(state_reg, clock.pos())    // clock thread
  }
  
};

#endif /* end _DH_HW_MULT_H_ */