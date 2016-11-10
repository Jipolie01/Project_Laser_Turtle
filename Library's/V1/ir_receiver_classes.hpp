#include "hwlib.hpp"
#include "test_class.hpp"

#ifndef IR_RECEIVER_CLASSES_HPP
#define IR_RECEIVER_CLASSES_HPP

class ir_receiver{
private:
    hwlib::pin_in & IR_receiver_pin;
    hwlib::pin_out & gnd;
    hwlib::pin_out & vcc;
    
public:
    ir_receiver(hwlib::pin_in & IR_receiver_pin, hwlib::pin_out & gnd, hwlib::pin_out & vcc):
        IR_receiver_pin(IR_receiver_pin),
        gnd(gnd),
        vcc(vcc)
    {
        gnd.set(0);
        vcc.set(1);
    }
    
   ///Get one bit from the bitstream
    //
    ///This function gets one bit from the incoming bitstream and returns a boolean to
    /// confirm if it is a one or a zero. It does not need any parameters. The bit is
    /// received according to the above mentioned document.
    bool get_bit(){
        if ( IR_receiver_pin.get() == 0){
            hwlib::wait_us(1000);
            if (IR_receiver_pin.get() == 0){
                hwlib::wait_us(700);
                return 1;
            }
            else if (IR_receiver_pin.get() == 1){
                return 0;
            }
        }
        return 0;
    }
    
    ///Get bits from IR receiver
    //
    ///This functions get fifteen bits from the IR receiver and puts them in an integer.
    ///This integer is then returned so it can be decoded by the decoder. The function
    /// dus not need any parameters.
    char16_t get_bits(){
        char16_t bitstream = 1;
        bitstream = bitstream << 1;
        for(int i = 0; i < 15; i++){
            bitstream = (bitstream | get_bit());
            bitstream = bitstream << 1;
            /*if(i != 30){
                byte = byte << 1;
            }*/
            
            //i++;
        }
        return bitstream;
    }
    
};

///Class for receiving IR message
//
///This class has been created to easily receive an IR message. It uses the protocol specified
/// in the document "V2THDE - Casus lasertag 2016-2017.pdf". The class uses hwlib created
// by Wouter van Ooijen to make programming in c++ for Arduino and other microcontrollers
/// more easy.
class receive_ir_controller : public rtos::task<>{
private:
    test_class & run_game;
    ir_receiver receiver;
    rtos::clock check_receiver_clock;
    
    void main(){
        while(1){
            wait(check_receiver_clock);
            if(receiver.get_bit()){
                auto value = receiver.get_bits();
                run_game.write(value);
                hwlib::cout << "receiving something\n";
            }
        }
    }
public:
    ///Default constructor
    //
    ///The default constructor needs a pin to wich the IR reciever has been attached.
    receive_ir_controller(test_class & run_game, hwlib::pin_in & IR_receiver_pin, hwlib::pin_out & gnd, hwlib::pin_out & vcc)://test class needs to be run game
        run_game(run_game),
        receiver ( IR_receiver_pin, gnd, vcc),
        check_receiver_clock(this, 27*rtos::us, "check_receiver_clock")
    {}
    
    ///Print decoded information
    //
    ///This function prints the decoded information after receiving a bitstream. It takes
    /// no parameters and returns nothing.
    /*void get_decoded_bits(){
        char16_t bitstream = get_bits();
        while(decoder(bitstream)){
            bitstream = get_bits();
        }
        hwlib::cout << "Identifier you\'ve been hit by: " << received_player_id << "\n";
        hwlib::cout << "Weapon you\'ve been hit by: " << received_weapon_id << "\n";
    }*/
};

#endif // IR_RECEIVER_CLASSES
