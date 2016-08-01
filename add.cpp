//******************************************************************
// add.cpp - processes for adder
// Author: Nick Birch
// Date: July 30, 2016
//******************************************************************
#include "systemc.h"
#include "modules.h"

void add::add_process() {
    
    // temporary variables
    sc_uint<NN_HALF_DIGIT_BITS> a_in_temp = A_IN.read();
    sc_uint<NN_HALF_DIGIT_BITS> b_in_temp = B_IN.read();
    sc_uint<NN_DIGIT_BITS> sum_temp = a_in_temp + b_in_temp;

    // check for carry out bit and set or reset carry_OUT flag accordingly
    if(sum_temp > MAX_NN_HALF_DIGIT){
    	//if(sum_temp > 15)
    	carry_OUT.write(1);
    }
    else{
    	carry_OUT.write(0);
    }

    // write result to output port
    Z_OUT.write(a_in_temp + b_in_temp);
}