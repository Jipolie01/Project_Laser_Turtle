#include "hwlib.hpp"

#ifndef RECEIVE_IR_MESSAGE_HPP
#define RECEIVE_IR_MESSAGE_HPP

class receive_ir_message
{
private:
        hwlib::target::pin_in &receiver_pin;
        int received_id;
        int received_weapon;
public:
    receive_ir_message(hwlib::target::pin_in &receiver_pin);
    ~receive_ir_message();
    bool get_bit();
    int get_byte();
    int decoder(int receiver_byte);
    void get_decoded_byte();
};

#endif // RECEIVE_IR_MESSAGE_HPP
