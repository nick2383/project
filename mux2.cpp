//******************************************************************
// mux2.cpp - processes for mutliplexer
// Author: Nick Birch
// Date: July 30, 2016
//******************************************************************
#include "systemc.h"
#include "modules.h"

void mux2::mux2_process() {
    
    if (sel.read() == SC_LOGIC_1) {
        Z_OUT.write(A_IN.read());
    }
    else {
        Z_OUT.write(B_IN.read());
    }

    //cout << "\n mux A_IN = " << A_IN.read();
    //cout << "\n mux B_IN = " << B_IN.read();
}