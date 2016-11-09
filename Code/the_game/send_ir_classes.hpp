/// @file
#include "hwlib.hpp"
#include "entity_classes.hpp"
#include "rtos.hpp"

#ifndef SEND_IR_CLASSES_HPP
#define SEND_IR_CLASSES_HPP

///Ir sender class
//
///This class is a boundary object and is used to send bits
/// in the way specified in the protocal. It contains a 36khz
/// pin wich is arduino pin digital 2.
class ir_sender{
private:
    hwlib::target::d2_36kHz sender_pin;
public:
    ///Send one over pin
    //
    ///This function sends a one over the sender pin.
    /// It returns nothing and dus not requires
    /// any parameters.
    void send_one(){
        sender_pin.set(1);
        hwlib::wait_us(1600);
        sender_pin.set(0);
        hwlib::wait_us(800);
    }
    
    ///Send zero over pin
    //
    ///This function sends a zero over the sender pin.
    /// It returns nothing and dus not requires
    /// any parameters.
    void send_zero(){
        sender_pin.set(1);
        hwlib::wait_us(800);
        sender_pin.set(0);
        hwlib::wait_us(1600);
    }
    
    ///Turn sender pin off
    //
    ///This function sets the sender pin to zero.
    ///It dus not need any parameters nor dus it return
    /// anything.
    void set_zero(){
        sender_pin.set(0);
    }
    
    ///Send a full message
    //
    ///This function sends the complete 16 bit message.
    ///It receives the message as paramter and returns nothing.
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


///Ir send controller
//
///An object of this class can be created to send bits
/// using the in the casus specified protocol.
class send_controller : public rtos::task<>{
private:
    ir_sender ir_send;
    my_player_information & message_to_send;
    rtos::flag send_message_flag;
    
    ///Main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. The function waits for the send message
    /// flag to be set and then sends the message.
    void main(){
        while(1){
            wait(send_message_flag);
            send_full_message();
        }
    }
    
    ///Send the message
    //
    ///This function is called when a message is needed to be send.
    ///It requires no parameters nor dus it return anything.
    ///It reads the compiled bits from the player information entity
    /// and sends these to the send message function for sending.
    void send_full_message(){
        auto message = message_to_send.get_compiled_bits();
        ir_send.send_message(message);
    }
public:
    ///Default constructor
    //
    ///This constructor receives a reference to the player information
    /// as a parameters and initializes all the variables.
    send_controller(my_player_information & player_information):
    task(3, "send_task"),
    ir_send(),
    message_to_send(player_information),
    send_message_flag(this, "send_message_flag")
    {}
    
    ///Set the send message flag
    //
    ///This function makes other objects able to set the send message flag.
    void enable_flag(){
        send_message_flag.set();
    }
};
#endif //SEND_IR_CLASSES_HPP