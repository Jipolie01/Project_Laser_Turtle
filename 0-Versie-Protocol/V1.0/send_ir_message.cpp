#include "send_ir_message.hpp"

send_ir_message::send_ir_message(hwlib::target::d2_36kHz  & sender_pin,  int identifier = 1, int weapon = 1):
sender_pin( sender_pin ),
identifier ( identifier ),
weapon( weapon ),
sending_byte(encoder())
{}

send_ir_message::~send_ir_message()
{}

void send_ir_message::send_one()
{
    sender_pin.set(1);
    hwlib::wait_us(1600);
    sender_pin.set(0);
    hwlib::wait_us(800);
}

void send_ir_message::send_zero()
{
    sender_pin.set(1);
    hwlib::wait_us(800);
    sender_pin.set(0);
    hwlib::wait_us(1600);
}

void send_ir_message::send_byte()
{
    for(int amount_messages = 0; amount_messages < 2; amount_messages++){
        for(int i = 15; i >= 0; i--){
            if((sending_byte & (1 << i)) != 0){
                send_one();
            }else{
                send_zero();
            }
        }
        
        hwlib::wait_ms(3);
    }
}

int send_ir_message::encoder()
{
    int sending_byte = 1;
    sending_byte= sending_byte << 5;
    sending_byte = (sending_byte | identifier);
    sending_byte = sending_byte << 5;
    sending_byte = (sending_byte | weapon);
    sending_byte = sending_byte << 5;
    sending_byte = (sending_byte | (identifier ^ weapon));
    return sending_byte;
}