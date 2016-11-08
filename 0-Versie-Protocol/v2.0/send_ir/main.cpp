#include "hwlib.hpp"
#include "send_ir_classes.hpp"
#include "rtos.hpp"
#include "entity_classes.hpp"
#include "test_class.hpp"

int main(){
    //Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    //Defining pins
    //auto button = hwlib::target::pin_in(hwlib::target::pins::d7);
    
    //Defining class objects
    auto player_information = my_player_information();
    auto controller = ir_message_controller(player_information);
    auto test_controller = test_class(controller, player_information);
    
    rtos::run();
}