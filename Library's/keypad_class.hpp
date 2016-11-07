#include "hwlib.hpp"

//class keypad
///make a class named keypad and define the pins you are going to use.
class Keypad{
private:
    hwlib::matrix_of_switches matrix;
    hwlib::keypad< 16 > keypad;
public:
//constructor for object keypad
    Keypad(hwlib::port_oc_from_pins & out_port, hwlib::port_in_from_pins & in_port):
        matrix(out_port, in_port),
        keypad(matrix, "123A456B789C*0#D")
    {}
    
//check_for_input function
///get a character from keypad.
    char check_for_input(){
        return keypad.getc();
    }
};