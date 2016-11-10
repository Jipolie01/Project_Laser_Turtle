#include "hwlib.hpp"
#include "entity_classes.hpp"
#include "rtos.hpp"

#ifndef SEND_IR_CLASSES_HPP
#define SEND_IR_CLASSES_HPP

class ir_sender{
private:
    hwlib::target::d2_36kHz sender_pin;
public:    
    void send_one(){
        sender_pin.set(1);
        hwlib::wait_us(1600);
        sender_pin.set(0);
        hwlib::wait_us(800);
    }
    
    void send_zero(){
        sender_pin.set(1);
        hwlib::wait_us(800);
        sender_pin.set(0);
        hwlib::wait_us(1600);
    }
    
    void set_zero(){
        sender_pin.set(0);
    }
    
    void send_message(char16_t compiled_message){
        for(int amount_messages = 0; amount_messages < 2; amount_messages++){
            for(int i = 15; i >= 0; i--){
                if((compiled_message & (1 << i)) != 0){
                    send_one();
                }
                else{
                    send_zero();
                }
            }
            sender_pin.set(0);
            hwlib::wait_ms(3);
        }
        
}
};


class send_controller : public rtos::task<>{
private:
    ir_sender ir_send;
    my_player_information & message_to_send;
    rtos::flag send_message_flag;
    
    void main(){
        while(1){
            wait(send_message_flag);
            send_full_message();
        }
    }
    void send_full_message(){
        auto message = message_to_send.get_compiled_bits();
        ir_send.send_message(message);
    }
public:
    send_controller(my_player_information & player_information):
    task(3, "send_task"),
    ir_send(),
    message_to_send(player_information),
    send_message_flag(this, "send_message_flag")
    {}

    void enable_flag(){
        send_message_flag.set();
    }
};
#endif //SEND_IR_CLASSES_HPP