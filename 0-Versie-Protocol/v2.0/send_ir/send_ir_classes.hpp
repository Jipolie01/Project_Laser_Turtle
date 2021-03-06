#include "hwlib.hpp"
#include "rtos.hpp"
#include "entity_classes.hpp"

#ifndef SEND_IR_CLASSES_HPP
#define SEND_IR_CLASSES_HPP

class ir_sender{
private:
    hwlib::target::d2_36kHz IR_led_pin;
public:
    ir_sender()
    {}
    
    void set(bool val){
        if(val){
            IR_led_pin.set(1);
            hwlib::wait_us(1600);
            IR_led_pin.set(0);
            hwlib::wait_ms(800);
        }
        else{
            IR_led_pin.set(1);
            hwlib::wait_us(800);
            IR_led_pin.set(0);
            hwlib::wait_ms(1600);
        }
    }
    
    ///Sending a message
    //
    ///This function sends the endcoded message wich was created in the default constructor.
    ///It requires no parameters and returns nothing. It just sends the message twice. This
    /// happens for acuracy.
    void send_message(char16_t compiled_message){
        for(int amount_messages = 0; amount_messages < 2; amount_messages++){
            for(int i = 15; i >= 0; i--){
                if((compiled_message & (1 << i)) != 0){
                    set(1);
                }
                else{
                    set(0);
                }
            }
            hwlib::wait_ms(3);
        }
    }
};

///Class for sending IR message
//
///This class has been created to easily send an IR message. It uses the protocol specified
/// in the document "V2THDE - Casus lasertag 2016-2017.pdf". The class uses hwlib created
// by Wouter van Ooijen to make programming in c++ for Arduino and other microcontrollers
/// more easy.
class ir_message_controller : public rtos::task<>{
private:
    ir_sender ir_led;
    my_player_information & player_information;
    rtos::flag send_message_flag;
    
    void main(){
        while(1){
            wait(send_message_flag);
            hwlib::cout << "char16_t as integer: " << player_information.get_compiled_bits() << '\n';
            ir_led.send_message(player_information.get_compiled_bits());
        }
    }
public:
    ///Default constructor
    //
    ///The default constructor of this class needs to receive a pin the IR led is attached to,
    /// a player identifier and a weapon identifier. These are all used to create the commands
    /// we are sending using the IR protocol. The default constructor also immediatly encodes
    /// the values received as parameters into the encode function (see below) to create the
    /// message that can be send.
    //send_ir_message(hwlib::target::d2_36kHz & IR_led_pin, byte player_id = 0, byte weapon_id = 0):
    ir_message_controller(my_player_information & player_information):
        player_information(player_information),
        send_message_flag(this, "send_message_flag")
        
    {}
    
    void enable_flag(){
        send_message_flag.set();
    }
};

#endif // SEND_IR_CLASSES_HPP
