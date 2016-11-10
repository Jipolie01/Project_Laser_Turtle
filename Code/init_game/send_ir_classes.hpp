/// @file
#include "hwlib.hpp"
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
    rtos::channel<char16_t, 6> messages_channel;
    
    ///Main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. The function waits for the messages
    /// channel to receive a message to be send.
    void main(){
        while(1){
            wait(messages_channel);
            send_full_message();
        }
    }
    
    ///Send the message
    //
    ///This function is called when a message is needed to be send.
    ///It requires no parameters nor dus it return anything.
    ///It reads the compiled bits from the messages channel
    /// and sends them to the send message function for sending.
    void send_full_message(){
        auto message = messages_channel.read();
        ir_send.send_message(message);
    }
public:
    ///Default constructor
    //
    ///The constructor initializes all the objects of this class
    /// and creates a ir sender object.
    send_controller():
        task(0, "send_task"),
        ir_send(),
        messages_channel(this, "messages_channel")
    {}
    
    ///Write to the messages channel
    //
    ///This function makes other objects able to wirte char16_t values to
    /// the messages channel.
    void write(char16_t value){
        messages_channel.write(value);
    }
};
#endif //SEND_IR_CLASSES_HPP