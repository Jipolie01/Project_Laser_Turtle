#include "ir_receiver_classes.hpp"
#include "hwlib.hpp"

bool decode(char16_t receiving_bits, byte & received_player_id, byte & received_weapon_id){
        byte identifier_player;
        byte identifier_weapon;
        byte exor;
        
        for(int i = 14; i >= 10; i--){
            if((receiving_bits & (1 << i)) != 0){
                identifier_player = (identifier_player| 1);
            }
            else{
                identifier_player = (identifier_player | 0);
            }
            if(i != 10){
            identifier_player = identifier_player << 1;
            }
        }
        
        for(int i = 9; i >= 5; i--){
            if((receiving_bits & (1 << i)) != 0){
                identifier_weapon = (identifier_weapon |1);
            }
            else{
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
        }
        else{
                return 1;
        }
    }

int main(void){
    hwlib::target::pin_out vcc = hwlib::target::pin_out(hwlib::target::pins::d10);
    hwlib::target::pin_out gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    hwlib::target::pin_in data = hwlib::target::pin_in(hwlib::target::pins::d8);
    byte player_id;
    byte weapon_id;
    byte play_time;
    ir_receiver_controller receiver(data, gnd, vcc);
	while(1){
        int start_bit = receiver.get_start_bit();
            //check if return bit == 1
            if(start_bit == 1){
                auto bitstream = receiver.receive_full_message();
                if(bitstream != -1){
                    if(decode(bitstream, player_id, weapon_id)){
                        if(player_id == 0){
                            play_time = weapon_id;
                            hwlib::wait_ms(500);
                        }
                        else{
                            continue;
                        }
                    }
                }
                else{
                    continue;
                }
            }
    }
    hwlib::cout << play_time;
}
