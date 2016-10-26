#include "hwlib.hpp"

//class keypad
///make a class named keypad and define the pins you are going to use.
class Keypad{
private:
    hwlib::target::pin_oc out0 = hwlib::target::pin_oc( hwlib::target::pins::a0 );
    hwlib::target::pin_oc out1 = hwlib::target::pin_oc( hwlib::target::pins::a1 );
    hwlib::target::pin_oc out2 = hwlib::target::pin_oc( hwlib::target::pins::a2 );
    hwlib::target::pin_oc out3 = hwlib::target::pin_oc( hwlib::target::pins::a3 );
    hwlib::port_oc_from_pins out_port = hwlib::port_oc_from_pins( out0, out1, out2, out3 );
    hwlib::target::pin_in in0 = hwlib::target::pin_in( hwlib::target::pins::a4 );
    hwlib::target::pin_in in1 = hwlib::target::pin_in( hwlib::target::pins::a5 );
    hwlib::target::pin_in in2 = hwlib::target::pin_in( hwlib::target::pins::a6 );
    hwlib::target::pin_in in3 = hwlib::target::pin_in( hwlib::target::pins::a7 );
    hwlib::port_in_from_pins in_port = hwlib::port_in_from_pins( in0, in1, in2, in3 );
public:
//constructor for object keypad
    Keypad(){}
    
//check_for_input function
///define keypad and matrix.
///get a character from keypad.
    char check_for_input(){
        auto matrix = hwlib::matrix_of_switches(out_port, in_port);
        auto keypad = hwlib::keypad< 16 >(matrix, "123A456B789C*0#D");
        auto c = keypad.getc();
        return c;
    }
};

int main(int argc, char **argv){
    
    Keypad keypad;
    while(1){
        auto c = keypad.check_for_input();
        hwlib::cout << " [" << c << "]\n";
        hwlib::wait_ms(200);
    }
	return 0;
}
