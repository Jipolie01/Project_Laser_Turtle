#include "receive_ir_message.hpp"

receive_ir_message::receive_ir_message(hwlib::target::pin_in &receiver_pin):
receiver_pin ( receiver_pin )
{}

receive_ir_message::~receive_ir_message()
{}

bool receive_ir_message::get_bit()
{
    for(;;){
        if ( receiver_pin.get() == 0){
            hwlib::wait_us(1000);
            if (receiver_pin.get() == 0){
                hwlib::wait_us(700);
                return 1;
            }
            else if (receiver_pin.get() == 1){
                return 0;
            }
        }
    }
}

int receive_ir_message::get_byte()
{
    int byte = 0;
    for(int i = 0; i <= 30; i++){
        byte = (byte | get_bit());
        if(i != 30){
            byte = byte << 1;
        }
        i++;
    }
    return byte;
}

int receive_ir_message::decoder(int receiving_byte)
{
    int identifier_player= 0;
    int identifier_weapon= 0;
    int exor = 0;
    for(int i = 14; i >= 10; i--){
        if((receiving_byte&(1 << i)) != 0){
            identifier_player = (identifier_player| 1);
        }else{
            identifier_player = (identifier_player | 0);
        }
        if(i != 10){
        identifier_player = identifier_player << 1;
        }
    }
    for(int i = 9; i >= 5; i--){
        if((receiving_byte& (1 << i)) != 0){
            identifier_weapon = (identifier_weapon |1);
        }else{
            identifier_weapon = (identifier_weapon | 0);
        }
        if(i != 5){
            identifier_weapon = identifier_weapon << 1;
        }
    }
    for(int i= 4; i >= 0; i--){
        if((receiving_byte& (1 << i)) != 0){
            exor = (exor | 1);
        }else{
            exor = (exor | 0);
        }
        if(i != 0){
            exor = exor<< 1;
        }
    }
    if((identifier_player ^ identifier_weapon) == exor){
            received_id = identifier_player;
            received_weapon = identifier_weapon;
            return 0;
    }else{
            return 1;
    }
    
}

void receive_ir_message::get_decoded_byte()
{
    int byte = get_byte();
    while(decoder(byte)){
        byte = get_byte();
    }
    hwlib::cout << "Identifier you\'ve been hit by: " << received_id << "\n";
    hwlib::cout << "Weapon you\'ve been hit by: " << received_weapon << "\n";
}
