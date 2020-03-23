// SystemC_FSM.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include "systemc.h"

/*
	Zustände der Fußgängerampel
*/
enum trafficLigth {
	greenCar, yellow1, redCar, greenWalker, redWalker, yellow2
};

SC_MODULE(fsm) {
	sc_in<bool> pressed, clk;
	sc_out<bool> carRed, carYellow, carGreen, walkerRed, walkerGreen;
	sc_signal<trafficLigth> state;
	sc_signal<bool> turn;

	void countdown(int& time, sc_time_unit unit) {
		cout << "started" << endl;
		wait(time, unit);
		time = 0;
	}

	void getNextState() {
		if (clk.posedge() && clk.read() == 1) {
			switch (state)
			{
			case greenCar:
				if (pressed) {
					state.write(yellow1);// = yellow1;
					next_trigger(10, SC_NS);
				}
				else {
					state = greenCar;
				}
				break;
			case yellow1:
				state = redCar;
				cout << "Yellow 1" << endl;
				next_trigger(10, SC_NS);
				break;
			case redCar:
				state = greenWalker;
				cout << "redCar" << endl;
				next_trigger(10, SC_NS);
				break;
			case greenWalker:
				state = redWalker;
				cout << "Green Wlaker" << endl;
				next_trigger(10, SC_NS);
				break;
			case redWalker:
				state = yellow2;
				cout << "red Walker" << endl;
				next_trigger(10, SC_NS);
				break;
			case yellow2:
				state = greenCar;
				cout << "Yellow 2" << endl;
				next_trigger(10, SC_NS);
				break;
			}
		}
	}

	void setState() {
		int waitTime = 5;
		sc_time_unit t = SC_NS;
		switch (state)
		{
		case greenCar:
			carRed = false;
			carYellow = false;
			carGreen = true;

			walkerGreen = false;
			walkerRed = true;
			break;
		case yellow1:

			carRed = false;
			carYellow = true;
			carGreen = false;

			walkerGreen = false;
			walkerRed = true;

			break;
		case redCar:

			carRed = true;
			carYellow = false;
			carGreen = false;

			walkerGreen = false;
			walkerRed = true;

			break;
		case greenWalker:

			carRed = true;
			carYellow = false;
			carGreen = false;

			walkerGreen = true;
			walkerRed = false;

			break;
		case redWalker:

			carRed = true;
			carYellow = false;
			carGreen = false;

			walkerGreen = false;
			walkerRed = true;

			break;
		case yellow2:

			carRed = true;
			carYellow = true;
			carGreen = false;

			walkerGreen = false;
			walkerRed = true;

			break;
		}
	}
	
	/*
	Konstruktor
	*/
	SC_CTOR(fsm) {
		SC_METHOD(getNextState);
		sensitive << clk;

		/*
		Sensitivität state: da wechsel des Zustands
		Sensitivität clk: da zeitabhängiger Zustandsübergang, kann entfallen wenn keine Zeitabhängigkeit
		*/
		SC_METHOD(setState);
		sensitive << state << clk;
	}
};

SC_MODULE(fsm_test) {
	sc_event ev1, ev2, ev3, ev4;
	fsm stateM;
	sc_signal<bool> button, redC, yellowC, greenC, redW, greenW;
	sc_clock TestClk;

	void trigger_thread() {
		wait(SC_ZERO_TIME);
		ev1.notify(5, SC_NS);
		ev2.notify(9, SC_NS);
		ev3.notify(190, SC_NS);
		ev4.notify(200, SC_NS);
		wait(20, SC_NS);
	}

	void watcher_thread() {

		/* Erstellt vcd Datei um Waveform auszuwerten
		grafische ansicht u.a. mit Programm GTKWave*/
		sc_trace_file* file;
		file = sc_create_vcd_trace_file("Ampel_fsm");
		sc_trace(file, button, "Button");
		sc_trace(file, redC, "ROT_A");
		sc_trace(file, yellowC, "GELB_A");
		sc_trace(file, greenC, "GRUEN_A");
		sc_trace(file, redW, "ROT_F");
		sc_trace(file, greenW, "GRUEN_F");
		sc_trace(file, TestClk, "CLK");
		wait(ev1);
		cout << "Event occured" << sc_time_stamp() << endl;
		button.write(1);
		wait(ev2);
		cout << "Event occured" << sc_time_stamp() << endl;
		button.write(0);
		wait(ev3);
		cout << "Event occured" << sc_time_stamp() << endl;
		button.write(1);
		wait(ev4);
		cout << "Event occured" << sc_time_stamp() << endl;
		button.write(0);

		wait(500, SC_NS);
		sc_stop();
		sc_close_vcd_trace_file(file);

	}

	SC_CTOR(fsm_test) : stateM("stateMaschine"), TestClk("TestClk", 1, 0.5, 0, true) {
		SC_THREAD(trigger_thread);
		SC_THREAD(watcher_thread);

		stateM.clk(TestClk);
		stateM.pressed(button);
		stateM.carRed(redC);
		stateM.carYellow(yellowC);
		stateM.carGreen(greenC);
		stateM.walkerGreen(greenW);
		stateM.walkerRed(redW);
	};
};

int sc_main(int argc, char* argv[])
{
	fsm_test test("Test");
	sc_start();

	return (0);
}