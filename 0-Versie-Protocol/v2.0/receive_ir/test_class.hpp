#include "hwlib.hpp"
#include "rtos.hpp"
#include "application_logic_classes.hpp"
#include "entity_classes.hpp"

#ifndef TEST_CLASS_HPP
#define TEST_CLASS_HPP

class test_class : public rtos::task<>{
private:
    rtos::channel<char16_t, 5> received_information_channel;
    ir_message_logic message_logic;
    byte player_id;
    byte weapon_id;
    int hits_array_pos = 0;
    hit * hits[100];
    
    void add_hit(hit * h){
        
    }
    
    void main(void){
        hwlib::cout << "Starting program .. \n";
        while(1){
            wait(received_information_channel);
            hwlib::cout << "channel event\n";
            auto information = received_information_channel.read();
            if(message_logic.decode(information, player_id, weapon_id)){
                hit h(player_id, weapon_id);
                hits[hits_array_pos] = &h;
                hit tmp = *hits[hits_array_pos];
                hwlib::cout << "player_id: " << tmp.get_player_id() << '\n';
                hwlib::cout << "weapon id: " << tmp.get_weapon_id() << '\n';
                hits_array_pos++;
            }
        }
    }
public:
    test_class():
        task("test_class"),
        received_information_channel(this, "received_information_channel")
    {}
    
    void write(char16_t value){
        received_information_channel.write(value);
    }
};

#endif // TEST_CLASS_HPP
