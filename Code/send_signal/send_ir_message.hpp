#include "hwlib.hpp"

#ifndef SEND_IR_MESSAGE_HPP
#define SEND_IR_MESSAGE_HPP

class send_ir_message
{
private:
    hwlib::target::d2_36kHz  & sender_pin;
    int identifier;
    int weapon;
    int sending_byte;
public:
    send_ir_message(hwlib::target::d2_36kHz  & sender_pin, int identifier, int weapon);
    ~send_ir_message();
    void send_one();
    void send_zero();
    void send_byte();
    int encoder();
};

#endif // SEND_IR_MESSAGE_HPP
