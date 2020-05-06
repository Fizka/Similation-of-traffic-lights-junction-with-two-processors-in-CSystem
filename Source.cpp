#include "systemc.h"

SC_MODULE(Processor1) {
	sc_fifo_out<int> sliders;
	sc_fifo_out<int> state;
	sc_out<bool> error_flag;
	sc_out<bool> led1;
	sc_out<bool> led2;
	sc_out<bool> led4;
	sc_out<bool> led5;
	sc_out<bool> led3;
	sc_out<bool> led6;

	int sw = 0;
	bool runnedPrograms[7] = { false };
	bool showedConfirm;
	bool errorFlag;

	void task1() {

		while (1) {
			cin >> sw;
			if (sw > 0 || sw < 8) {
				showedConfirm = false;
				runnedPrograms[sw - 1] = !runnedPrograms[sw - 1];
				errorFlag = false;
				int error = 0;
				for (int i = 0; i < 5; i++) {
					if (runnedPrograms[i] == true)
						++error;
					if (error > 1)
						errorFlag = true;
				}
				if (runnedPrograms[sw - 1] == false)
					sw = -sw;

				error_flag.write(errorFlag);
				sliders.write(sw);
				state.write(sw);
				wait(50, SC_NS);
			}
			if (sw == 1) {
				wait(10, SC_NS);
				cout << "Wjazd na skrzyzowanie 1" << endl;
			}
			if (sw == -1) {

				led5.write(false);
				led2.write(false);
				led3.write(false);
				wait(10, SC_NS);
				cout << "Wyjazd ze skrzyzowania 1" << endl;
			}

		}
	}

	void task2() {
		while (1) {
			wait(3, SC_NS);
			if (sw == 2 && !showedConfirm) {
				showedConfirm = true;

				wait(10, SC_NS);
				cout << "Wjazd na skrzyzowanie 2" << endl;
			}
			else if (sw == -2 && !showedConfirm) {
				showedConfirm = true;
				led5.write(false);
				led2.write(false);
				led3.write(false);
				wait(10, SC_NS);
				cout << "Wyjazd ze skrzyzowania 2" << endl;
			}
		}
	}

	void taskSwiatla() {

		while (1) {
			wait(3, SC_NS);
			if (sw == 6 && !showedConfirm) {
				showedConfirm = true;
				led1.write(true);
				led6.write(true);
				wait(10, SC_NS);
				cout << "Swiatla Zielone" << endl;
			}
			else if ((sw == -6) && (!showedConfirm)) {
				showedConfirm = true;
				led1.write(false);
				led2.write(true);
				led5.write(true);
				wait(10, SC_NS);
				cout << "Swiatla pomaranczowe" << endl;

			}
			else if (sw == 7 && !showedConfirm) {
				showedConfirm = true;
				led2.write(false);
				led5.write(false);
				led6.write(false);
				led3.write(true);
				led4.write(true);
				wait(10, SC_NS);
				cout << "Swiatla Czerwone" << endl;

			}
			else if ((sw == -7) && (!showedConfirm)) {
				showedConfirm = true;
				led4.write(false);
				led2.write(true);
				led5.write(true);
				wait(10, SC_NS);
				cout << "Swiatla Pomaranczowo-Czerwone" << endl;
			}
		}
	}


	void task4() {
		while (1) {
			wait(3, SC_NS);
			if (sw == 3 && !showedConfirm) {
				showedConfirm = true;

				wait(10, SC_NS);
				cout << "Wjazd na skrzyzowanie 3" << endl;
			}
			else if (sw == -3 && !showedConfirm) {
				showedConfirm = true;
				led5.write(false);
				led2.write(false);
				led3.write(false);
				wait(10, SC_NS);
				cout << "Wyjazd ze skrzyzowania 3" << endl;
			}
		}
	}

	void task5() {

		while (1) {
			wait(3, SC_NS);
			if (sw == 4 && !showedConfirm) {
				showedConfirm = true;

				wait(10, SC_NS);
				cout << "Wjazd na skrzyzowanie 4" << endl;
			}
			else if (sw == -4 && !showedConfirm) {
				showedConfirm = true;
				led5.write(false);
				led2.write(false);
				led3.write(false);
				wait(10, SC_NS);
				cout << "Wyjazd ze skrzyzowania 4" << endl;
			}
		}
	}

	SC_CTOR(Processor1) {
		SC_THREAD(taskSwiatla);
		SC_THREAD(task1);
		SC_THREAD(task2);
		SC_THREAD(task4);
		SC_THREAD(task5);
	}
};

SC_MODULE(Processor2) {
	sc_fifo_in<int> sliders;
	sc_in<bool> error_flag;
	sc_out<bool> led6;

	int lastState = 0;
	int state = 0;

	void task6() {
		while (1) {
			wait(3, SC_NS);
			state = sliders.read();
			if (lastState != state) {
				if (state == 5) {
					wait(10, SC_NS);
					cout << "Wjazd na skrzyzowanie 5" << endl;
				}
				if (state == -5) {
					wait(10, SC_NS);
					cout << "Wyjazd ze skrzyzowania 5" << endl;
				}
				lastState = state;
			}
		}
	}

	SC_CTOR(Processor2) {
		SC_THREAD(task6);
	}
};

SC_MODULE(Module) {
	sc_fifo_in<int> sliders;
	sc_in<bool> error_flag;
	sc_out<bool> led3;
	sc_out<bool> ledError;

	int state = 0;
	int lastState = 0;

	void task3() {
		while (1) {
			wait(3, SC_NS);
			state = sliders.read();
			if (lastState != state) {
				if (error_flag) {
					wait(10, SC_NS);
					cout << "Err" << endl;
				}
				else {
					wait(10, SC_NS);
					cout << "  " << endl;
				}
				lastState = state;
			}
		}
	}

	SC_CTOR(Module) {
		SC_THREAD(task3);
	}
};

SC_MODULE(Bus) {
	sc_fifo<int> sliders;
	sc_fifo<int> state;
	sc_signal<bool> error_flag;
	sc_signal<bool> led1;
	sc_signal<bool> led2;
	sc_signal<bool> led3;
	sc_signal<bool> led4;
	sc_signal<bool> led5;
	sc_signal<bool> led6;
	sc_signal<bool> ledError;

	SC_CTOR(Bus) {

	}
};

SC_MODULE(Monitor) {
	sc_in<bool> led1;
	sc_in<bool> led2;
	sc_in<bool> led3;
	sc_in<bool> led4;
	sc_in<bool> led5;
	sc_in<bool> led6;
	sc_in<bool> ledError;

	void show() {
		//COMENT LINE UNDER TO SHOW HISTORY
		system("cls");
		cout << "LED1 2LED 3LED 4LED 5LED LED6  ERROR" << endl;
		cout << led1 << "    " << led2 << "    " << led3 << "    " << led4 << "    " << led5 << "    " << led6 << "    " << ledError << endl;
	}

	SC_CTOR(Monitor) {
		SC_METHOD(show);
		sensitive << led1;
		sensitive << led2;
		sensitive << led3;
		sensitive << led4;
		sensitive << led5;
		sensitive << led6;
		sensitive << ledError;
	}
};

int main(int argc, char** argv)
{
	Monitor m("monitor");
	Bus bus("b");
	Processor1 p1("proc1");
	Processor2 p2("proc2");
	Module mod("module");

	//leds channel
	p1.led1(bus.led1);
	p1.led2(bus.led2);
	p1.led4(bus.led4);
	p1.led5(bus.led5);
	p1.led3(bus.led3);
	p1.led6(bus.led6);

	mod.led3(bus.led3);
	mod.ledError(bus.ledError);
	p2.led6(bus.led6);
	m.led1(bus.led1);
	m.led2(bus.led2);
	m.led4(bus.led4);
	m.led5(bus.led5);
	m.ledError(bus.ledError);
	m.led3(bus.led3);
	m.led6(bus.led6);

	// state channel
	p1.sliders(bus.sliders);
	p1.state(bus.state);
	p2.sliders(bus.sliders);
	mod.sliders(bus.state);

	// error_flag channel
	p1.error_flag(bus.error_flag);
	p2.error_flag(bus.error_flag);
	mod.error_flag(bus.error_flag);

	sc_start();
	return 0;
}