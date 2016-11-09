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
    my_player_information &message_to_send;
    
    void main(){
        while(1){
            send_full_message();
            sleep(2000*rtos::ms);
        }
    }
public:
    send_controller(my_player_information & player_information):
    task("send_task"),
    ir_send(),
    message_to_send(player_information)
    {}

    void send_full_message(){
        char16_t message = message_to_send.get_compiled_bits();
        ir_send.send_message(message);
    }
};
/*
void send_one(auto sender_pin){
        sender_pin.set(1);
        hwlib::wait_us(1600);
        sender_pin.set(0);
        hwlib::wait_us(800);
    }
    
    void send_zero(auto sender_pin){
        sender_pin.set(1);
        hwlib::wait_us(800);
        sender_pin.set(0);
        hwlib::wait_us(1600);
    }

int main(void){
    //kill the watchdog
    WDT->WDT_MR=WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    hwlib::cout << "Starting program .. \n";
    auto player = my_player_information();
    player.set_compiled_bits(46774);
    auto sender = send_controller(player);
    
    rtos::run();
    
}*/

#endif //SEND_IR_CLASSES_HPP