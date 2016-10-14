#include "hwlib.hpp"

#ifndef RECEIVE_IR_MESSAGE_HPP
#define RECEIVE_IR_MESSAGE_HPP

///Class for receiving IR message
//
///This class has been created to easily receive an IR message. It uses the protocol specified
/// in the document "V2THDE - Casus lasertag 2016-2017.pdf". The class uses hwlib created
// by Wouter van Ooijen to make programming in c++ for Arduino and other microcontrollers
/// more easy.
class receive_ir_message
{
private:
        hwlib::target::pin_in & receiver_pin;///IR receiver pin
        int received_id;///received player identifier
        int received_weapon;///received weapon identifier
public:
    ///Default constructor
    //
    ///The default constructor needs a pin to wich the IR reciever has been attached.
    receive_ir_message(hwlib::target::pin_in & receiver_pin);
    
    ///Default destructor
    //
    ///The destructor releases the memory used by the class to be used for other objects.
    ~receive_ir_message();
    
    ///Get one bit from the bitstream
    //
    ///This function gets one bit from the incoming bitstream and returns a boolean to
    /// confirm if it is a one or a zero. It does not need any parameters. The bit is
    /// received according to the above mentioned document.
    bool get_bit();
    
    ///Get bits from IR receiver
    //
    ///This functions get fifteen bits from the IR receiver and puts them in an integer.
    ///This integer is then returned so it can be decoded by the decoder. The function
    /// dus not need any parameters.
    int get_byte();
    
    ///Decode the bitstream
    //
    ///This function decodes the bitstream in the way specified in the above mentioned
    /// document. It also checks the bitstream with an exor to make sure it is authentic.
    /// The function needs the bitstream as a parameter and returns zero or one depending
    /// on the result of the exor.
    int decoder(int receiver_byte);
    
    ///Print decoded information
    //
    ///This function prints the decoded information after receiving a bitstream. It takes
    /// no parameters and returns nothing.
    void get_decoded_byte();
};

#endif // RECEIVE_IR_MESSAGE_HPP
