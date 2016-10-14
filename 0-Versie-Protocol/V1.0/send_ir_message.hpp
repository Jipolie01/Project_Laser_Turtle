#include "hwlib.hpp"

#ifndef SEND_IR_MESSAGE_HPP
#define SEND_IR_MESSAGE_HPP

///Class for sending IR message
//
///This class has been created to easily send an IR message. It uses the protocol specified
/// in the document "V2THDE - Casus lasertag 2016-2017.pdf". The class uses hwlib created
// by Wouter van Ooijen to make programming in c++ for Arduino and other microcontrollers
/// more easy.
class send_ir_message
{
private:
    hwlib::target::d2_36kHz  & sender_pin; ///pin the IR led is attached to
    int identifier;
    int weapon;
    int sending_byte;///encoded message to send to the receiver
public:
    ///Default constructor
    //
    ///The default constructor of this class needs to receive a pin the IR led is attached to,
    /// a player identifier and a weapon identifier. These are all used to create the commands
    /// we are sending using the IR protocol. The default constructor also immediatly encodes
    /// the values received as parameters into the encode function (see below) to create the
    /// message that can be send.
    send_ir_message(hwlib::target::d2_36kHz  & sender_pin, int identifier, int weapon);
    
    ///Default destructor
    //
    ///The destructor frees up resources that have been used by this class so that other classes
    /// can use them.
    ~send_ir_message();
    
    ///Send a one over the IR pin
    //
    ///This function sends a one using the IR LED in the way specified in the above mentioned
    /// document. It takes no parameters and dus not return anything. It is here to make sending
    /// a one in the rest of the class more easy.
    void send_one();
    
    ///Send a zero over the IR pin
    //
    ///This function sends a zero using the IR LED in the way specified in the above mentioned
    /// document. It takes no parameters and dus not return anything. It just makes sending a
    /// zero much more easy.
    void send_zero();
    
    ///Sending a message
    //
    ///This function sends the endcoded message wich was created in the default constructor.
    ///It requires no parameters and returns nothing. It just sends the message twice. This
    /// happens for acuracy.
    void send_byte();
    
    ///Encode the message
    //
    ///This functions creates the message we send later. It dus not need any parameters.
    ///The return value is the final message that is send over the IR LED.
    int encoder();
};

#endif // SEND_IR_MESSAGE_HPP
