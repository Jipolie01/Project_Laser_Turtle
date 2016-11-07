#include "hwlib.hpp"
#include "rtos.hpp"
#include "send_ir_classes.hpp"
#include "entity_classes.hpp"

#ifndef TEST_CLASS_HPP
#define TEST_CLASS_HPP

class test_class : public rtos::task<>{
private:
    ir_message_controller & controller;
    my_player_information & player_information;
    ir_message_logic message_logic;
    
    void main(void){
        player_information.set_compiled_bits(message_logic.encoder(5,6));
        while(1){
            controller.enable_flag();
            sleep(5000*rtos::ms);
        }
    }
public:
    test_class(ir_message_controller & controller, my_player_information & player_information):
        task("test_class"),
        controller(controller),
        player_information(player_information)
    {}
};

#endif // TEST_CLASS_HPP
