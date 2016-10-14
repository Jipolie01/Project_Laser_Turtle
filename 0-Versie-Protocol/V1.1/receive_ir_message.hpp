#include "hwlib.hpp"

#ifndef RECEIVE_IR_MESSAGE_HPP
#define RECEIVE_IR_MESSAGE_HPP

///Class for receiving IR message
//
///This class has been created to easily receive an IR message. It uses the protocol specified
/// in the document "V2THDE - Casus lasertag 2016-2017.pdf". The class uses hwlib created
// by Wouter van Ooijen to make programming in c++ for Arduino and other microcontrollers
/// more easy.
class receive_ir_message {
private:
        hwlib::target::pin_in & IR_receiver_pin;
        byte received_player_id;
        byte received_weapon_id;
public:
    ///Default constructor
    //
    ///The default constructor needs a pin to wich the IR reciever has been attached.
    receive_ir_message(hwlib::target::pin_in & IR_receiver_pin):
        IR_receiver_pin ( IR_receiver_pin )
    {}
    
    ///Default destructor
    //
    ///The destructor releases the memory used by the class to be used for other objects.
    ~receive_ir_message()
    {}
    
    ///Get one bit from the bitstream
    //
    ///This function gets one bit from the incoming bitstream and returns a boolean to
    /// confirm if it is a one or a zero. It does not need any parameters. The bit is
    /// received according to the above mentioned document.
    bool get_bit(){
        for(;;){
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
        }
    }
    
    ///Get bits from IR receiver
    //
    ///This functions get fifteen bits from the IR receiver and puts them in an integer.
    ///This integer is then returned so it can be decoded by the decoder. The function
    /// dus not need any parameters.
    char16_t get_bits(){
        char16_t bitstream = 0;
        for(int i = 0; i < 16; i++){
            bitstream = (bitstream | get_bit());
            bitstream = bitstream << 1;
            /*if(i != 30){
                byte = byte << 1;
            }*/
            
            //i++;
        }
        return byte;
    }
    
    ///Decode the bitstream
    //
    ///This function decodes the bitstream in the way specified in the above mentioned
    /// document. It also checks the bitstream with an exor to make sure it is authentic.
    /// The function needs the bitstream as a parameter and returns zero or one depending
    /// on the result of the exor.
    bool decoder(int receiving_bits){
        byte identifier_player= 0;
        byte identifier_weapon= 0;
        byte exor = 0;
        for(int i = 14; i >= 10; i--){
            if((receiving_bits & (1 << i)) != 0){
                identifier_player = (identifier_player| 1);
            }else{
                identifier_player = (identifier_player | 0);
            }
            if(i != 10){
            identifier_player = identifier_player << 1;
            }
        }
        
        for(int i = 9; i >= 5; i--){
            if((receiving_bits & (1 << i)) != 0){
                identifier_weapon = (identifier_weapon |1);
            }else{
                identifier_weapon = (identifier_weapon | 0);
            }
            if(i != 5){
                identifier_weapon = identifier_weapon << 1;
            }
        }
        
        for(int i= 4; i >= 0; i--){
            if((receiving_bits & (1 << i)) != 0){
                exor = (exor | 1);
            }else{
                exor = (exor | 0);
            }
            if(i != 0){
                exor = exor<< 1;
            }
        }
        
        if((identifier_player ^ identifier_weapon) == exor){
                received_player_id = identifier_player;
                received_weapon_id = identifier_weapon;
                return 0;
        }else{
                return 1;
        }
    }
    
    ///Print decoded information
    //
    ///This function prints the decoded information after receiving a bitstream. It takes
    /// no parameters and returns nothing.
    void get_decoded_bits(){
        char16_t bitstream = get_bits();
        while(decoder(bitstream)){
            bitstream = get_bits();
        }
        hwlib::cout << "Identifier you\'ve been hit by: " << received_player_id << "\n";
        hwlib::cout << "Weapon you\'ve been hit by: " << received_weapon_id << "\n";
    }
};

#endif // RECEIVE_IR_MESSAGE_HPP
