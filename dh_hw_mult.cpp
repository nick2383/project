#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"

//*************************************************
// state register
void dh_hw_mult::state_reg() {
  while(1) {
    // reset the state machine
    if (reset.read() == SC_LOGIC_1) {
      state.write(S0_WAIT);
    }
    //
    else {
      state.write(next_state.read());
    }
    wait();
  }
}

//*************************************************
// state transition control
void dh_hw_mult::state_transition() {

  // default: self-loop
  next_state.write(state.read());
  
  switch (state.read()) {

    case S0_WAIT: 
        if(hw_mult_enable.read() == true) {
          next_state.write(S1_EXECUTE);
        }
        break;
  
    case S1_EXECUTE:
        next_state.write(S2_OUTPUT);
        break;

    case S2_OUTPUT:
        next_state.write(S3_FINISH);
        break;

    case S3_FINISH:
        if(hw_mult_enable.read() == false) {
          next_state.write(S0_WAIT);
        }
        break;
  
    default:
        break;
  }    
}

//*************************************************
// state output control
void dh_hw_mult::state_output() {

  NN_DIGIT a[2], b, c, t, u;
  NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;

  // defaults
  out_data_low.write(out_data_low.read());
  out_data_high.write(out_data_high.read());
  hw_mult_done.write(false);
  
  switch (state.read()) {

    case S0_WAIT:
        // deassert DONE 
        hw_mult_done.write(false);
        break;
  
    case S1_EXECUTE:
        // do multiplication
        // Read inputs
        
        reset.write(SC_LOGIC_1);
        reset.write(SC_LOGIC_0);
        multiplier.write(in_data_1.read());
        multiplicand.write(in_data_2.read());
        START.write(SC_LOGIC_1);


        // // Original code from NN_DigitMult()...   
        // bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
        // bLow = (NN_HALF_DIGIT)LOW_HALF (b);
        // cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
        // cLow = (NN_HALF_DIGIT)LOW_HALF (c);

        // a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
        // t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
        // u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
        // a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
      
        // if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
        // u = TO_HIGH_HALF (t);
      
        // if ((a[0] += u) < u) a[1]++;
        // a[1] += HIGH_HALF (t);
        
        // Write outputs
        while(FINISH == 0);
        out_data_low.write(HIGH_HALF(product));
        out_data_high.write(LOW_HALF(product));
        break;

    case S2_OUTPUT:
        // assert DONE
        hw_mult_done.write(true);
        break;

    case S3_FINISH:
        // keep DONE asserted
        hw_mult_done.write(true);
        break;
  
    default:
        break;
  }    
}


/*
void dh_hw_mult::process_hw_mult()
{
	
  NN_DIGIT a[2], b, c, t, u;
  NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;

  for (;;) {  
  
    if (hw_mult_enable.read() == true) 
    {	

    	// Read inputs	
    	b = in_data_1.read();
    	c = in_data_2.read();
    		
    	// Original code from NN_DigitMult()...		
    	bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
    	bLow = (NN_HALF_DIGIT)LOW_HALF (b);
    	cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
    	cLow = (NN_HALF_DIGIT)LOW_HALF (c);

    	a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
    	t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
    	u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
    	a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
    
    	if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
    	u = TO_HIGH_HALF (t);
    
    	if ((a[0] += u) < u) a[1]++;
    	a[1] += HIGH_HALF (t);
  		
     	// Hardware multiplication delay = 100 ns
      // removed timed waits
    	// wait (100, SC_NS);
    	
    	// Write outputs
    	out_data_low.write(a[0]);
    	out_data_high.write(a[1]);

      // assert done signal
      hw_mult_done.write(true);
		
    }

    wait();		// wait for a change of hw_mult_enable	

  }
	  	  
}
*/
