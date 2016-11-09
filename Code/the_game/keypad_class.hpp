/// @file
#include "hwlib.hpp"

#ifndef KEYPAD_CLASS_HPP
#define KEYPAD_CLASS_HPP

///Keypad class
//
///This class is an boundary object and creates a keypad from two sets of ports.
class Keypad{
private:
    hwlib::matrix_of_switches matrix;
    hwlib::keypad< 16 > keypad;
public:
    ///Default constructor
    //
    ///This constructor receives 2 parameters. Both of them are ports created from
    /// the keypad pins. The constructor uses these ports to create a keypad.
    Keypad(hwlib::port_oc_from_pins & out_port, hwlib::port_in_from_pins & in_port):
        matrix(out_port, in_port),
        keypad(matrix, "123A456B789C*0#D")
    {}
    
    ///Check for input
    //
    ///This function calls the getc() function from the keypad. It requires no
    /// parameters and returns the received character. The getc() function waits
    /// until a key has been pressed in a loop.
    char check_for_input(){
        return keypad.getc();
    }
};

#endif //KEYPAD_CLASS_HPP