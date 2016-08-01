#include "systemc.h"
#include "digit.h"
#include "dh_sw.h"
#include "dh_hw_mult.h"
#include "datapath.h"
#include "ctrl.h"


int sc_main(int argc , char *argv[])
{
	// clock declaration
	sc_clock clk ("clk", 100, SC_NS);

	// handshaking signal declarations
	sc_signal <bool> enable, done;
	sc_signal <NN_DIGIT> operand1, operand2, result1, result2;

	// datapath and controller signal declarations
	sc_signal<sc_uint<NN_HALF_DIGIT_BITS> > multiplier, multiplicand, HI_mux_zero, carry_mux_zero, LO_LSB, HI_LSB;
	sc_signal<sc_uint<NN_DIGIT_BITS> > product;
	
	sc_signal<sc_logic> HI_mux_sel, LO_mux_sel, HI_mux2_sel, carry_mux_sel; 
    sc_signal<sc_logic> HI_reg_load, LO_reg_load, rshift_load;
    sc_signal<sc_logic> START, FINISH;

	sc_signal<sc_logic> reset;



	
	enable.write(false);
	done.write(false);
	
	dh_sw DH_SW("DH_Software");
	DH_SW.out_data_1 (operand1);		// operand1 to hardware
	DH_SW.out_data_2 (operand2);		// operand2 to hardware
	DH_SW.in_data_low (result1);		// result1 from hardware
	DH_SW.in_data_high (result2);		// result2 from hardware
	DH_SW.hw_mult_enable (enable);		// enable hardware
	DH_SW.hw_mult_done (done);		// hardware done
	
	
	dh_hw_mult DH_HW_MULT("DH_Hardware_Multiplier");
	DH_HW_MULT.in_data_1 (operand1);	// operand1 from software
	DH_HW_MULT.in_data_2 (operand2);	// operand2 from software
	DH_HW_MULT.out_data_low (result1);	// result1 to software
	DH_HW_MULT.out_data_high (result2);	// result2 to software
	DH_HW_MULT.hw_mult_enable (enable);	// enable hardware
	DH_HW_MULT.hw_mult_done (done);		// hardware done
	DH_HW_MULT.clock (clk);				// hardware clock
	DH_HW_MULT.multiplier(multiplier);
	DH_HW_MULT.multiplicand(multiplicand);
	DH_HW_MULT.product(product);
	DH_HW_MULT.START(START);
	DH_HW_MULT.FINISH(FINISH);
	DH_HW_MULT.reset_mult(reset);


	// hardware multiplication interconnections
	datapath DP("DP"); 
	DP.reset(reset);
	DP.clock(clk);
	DP.multiplier(multiplier);
	DP.multiplicand(multiplicand);
	DP.HI_mux_zero(HI_mux_zero);
	DP.carry_mux_zero(carry_mux_zero);
	DP.product(product);
	DP.LO_LSB(LO_LSB);
	DP.HI_LSB(HI_LSB);
	DP.HI_mux_sel(HI_mux_sel);
	DP.LO_mux_sel(LO_mux_sel);
	DP.HI_mux2_sel(HI_mux2_sel);
	DP.carry_mux_sel(carry_mux_sel);
	DP.HI_reg_load(HI_reg_load);
	DP.LO_reg_load(LO_reg_load);
	DP.rshift_load(rshift_load);

	ctrl CTRL("CTRL");
	CTRL.reset(reset);
	CTRL.clock(clk);
	CTRL.LO_LSB(LO_LSB);
	CTRL.HI_mux_sel(HI_mux_sel);
	CTRL.LO_mux_sel(LO_mux_sel);
	CTRL.HI_mux2_sel(HI_mux2_sel);
	CTRL.carry_mux_sel(carry_mux_sel);
	CTRL.HI_reg_load(HI_reg_load);
	CTRL.LO_reg_load(LO_reg_load);
	CTRL.rshift_load(rshift_load);
	CTRL.START(START);
	CTRL.FINISH(FINISH);






	sc_start();

	return(0);
}