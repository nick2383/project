//******************************************************************
// splitter.cpp - bit splitter
// Author: Nick Birch
// Date: July 30, 2016
//******************************************************************
#include "systemc.h"
#include "modules.h"

void splitter::splitter_process() {
    
    // temporary variables
    sc_uint<NN_HALF_DIGIT_BITS> temp_lo_out = 0;
    sc_uint<NN_HALF_DIGIT_BITS> temp_hi_out = 0;

    // mask upper two bytes
    //temp_lo_out = A_IN.read() & 0x0F;
    temp_lo_out = LOW_HALF(A_IN.read());
    //temp_lo_out = A_IN.read() & 0x0000FFFF;
    // mask lowwer two bytes and shift right
    //temp_hi_out = (A_IN.read() & 0xF0) >> 4;
    temp_hi_out = HIGH_HALF(A_IN.read());
    // temp_hi_out = (A_IN.read() & 0xFFFF0000) >> NN_HALF_DIGIT_BITS;
    // write results to output ports
    HI_OUT.write(temp_hi_out);
    LO_OUT.write(temp_lo_out);
}
