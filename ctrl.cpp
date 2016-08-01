//******************************************************************
// ctrl.cpp - controller for datapath
// Author: Nick Birch
// Date: July 31, 2016
//******************************************************************
#include <systemc.h>
#include "ctrl.h"

void ctrl::state_reg() {
  while(1) {
    if (reset.read() == SC_LOGIC_1) {
      state.write(S0_BEGIN);
    }
    else state.write(next_state.read());
    wait();
  }
}

void ctrl::state_transition() {

  // default: self-loop
  next_state.write(state.read());
  
  switch (state.read()) {

    case S0_BEGIN:
        while(START.read() == SC_LOGIC_0);
        next_state.write(S1_COUNT);
        break;
	
    case S1_COUNT:
        if(counter >= (NN_HALF_DIGIT_BITS + 1)){
          next_state.write(S6_FINISH);
        }
        // if(counter == 32){
        //   next_state.write(S4_FINISH);
        // }
        else {
          next_state.write(S2_CHECK);
        }
        break;

    case S2_CHECK:
        if (LO_LSB.read() == 1) {
          next_state.write(S3_ADD);
        }
        else if (LO_LSB.read() == 0) {
          next_state.write(S3_DO_NOTHING);
        }
        break;

    case S3_ADD:
        next_state.write(S4_SHIFT);
        break;

    case S3_DO_NOTHING:
        next_state.write(S4_SHIFT);
        break;

    case S4_SHIFT:
        next_state.write(S5_WAIT);
        break;

    case S5_WAIT:
        next_state.write(S1_COUNT);
        break;

    case S6_FINISH:
        break;
	
    default:
        break;
  }    
}


void ctrl::state_output() {
  //default: all outputs at zero
  //HI_mux_sel.write(SC_LOGIC_1); 
  //LO_mux_sel.write(SC_LOGIC_1);
  // HI_mux2_sel.write(SC_LOGIC_0); 
  // carry_mux_sel.write(SC_LOGIC_0);
  // HI_reg_load.write(SC_LOGIC_0); 
  // LO_reg_load.write(SC_LOGIC_0);
  //rshift_load.write(SC_LOGIC_1);
  
  switch (state.read()) {

    case S0_BEGIN:
        //cout << "\n S0_BEGIN";
        HI_mux_sel.write(SC_LOGIC_0);
	      HI_reg_load.write(SC_LOGIC_1);
	      LO_mux_sel.write(SC_LOGIC_0);
        LO_reg_load.write(SC_LOGIC_1);
        carry_mux_sel.write(SC_LOGIC_0);
        HI_mux2_sel.write(SC_LOGIC_0);
        rshift_load.write(SC_LOGIC_0); 
        FINSIH.write(SC_LOGIC_0);   
        break;

    case S1_COUNT:
        //cout << "\n S1_COUNT counter = " << counter;
        counter = counter + 1;
        // HI_reg_load.write(SC_LOGIC_1);
        // LO_reg_load.write(SC_LOGIC_1);
        // rshift_load.write(SC_LOGIC_0); 
        break;

    case S2_CHECK:
        //cout << " \n S2_CHECK";
        // set muxes to always ON after init values have been loaded
        LO_mux_sel.write(SC_LOGIC_1);
        HI_mux_sel.write(SC_LOGIC_1);
        // close registers
        LO_reg_load.write(SC_LOGIC_0);
        HI_reg_load.write(SC_LOGIC_0);
        // rshift_load.write(SC_LOGIC_1);
        break;

    case S3_DO_NOTHING:
        //cout << " \n S3_DO_NOTHING";
        // set muxes for "Do Nothing" case
	      carry_mux_sel.write(SC_LOGIC_0);
	      HI_mux2_sel.write(SC_LOGIC_0);
        //rshift_load.write(SC_LOGIC_1);
        break;

    case S3_ADD:
        //cout << " \n S3_ADD";
        // set muxes for "ADD case" case
        carry_mux_sel.write(SC_LOGIC_1);
        HI_mux2_sel.write(SC_LOGIC_1); 
        // HI_reg_load.write(SC_LOGIC_0);
        // LO_reg_load.write(SC_LOGIC_0);
	      //rshift_load.write(SC_LOGIC_1);
        break;

    case S4_SHIFT:
        //cout << " \n S4_SHIFT";
        // perform shift
        rshift_load.write(SC_LOGIC_1);
        break;

    case S5_WAIT:
        // load registers with new values
        LO_reg_load.write(SC_LOGIC_1);
        HI_reg_load.write(SC_LOGIC_1);
        rshift_load.write(SC_LOGIC_0);
        //cout << " \n S5_WAIT";
        break;

    case S6_FINISH:
        //cout << " \n S6_FINISH";
        FINISH.write(SC_LOGIC_1);
        break;	
	
    default:
        break;
  }    
}