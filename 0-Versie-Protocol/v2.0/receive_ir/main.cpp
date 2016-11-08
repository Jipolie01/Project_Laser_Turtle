#include "hwlib.hpp"
#include "ir_receiver_classes.hpp"

int main(void){
    //Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    //Defining pins
    auto r_pin = hwlib::target::pin_in(hwlib::target::pins::d8);
    auto gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto vcc = hwlib::target::pin_out(hwlib::target::pins::d10);


    //Running program
    auto test_controller = test_class();
    auto controller = receive_ir_controller(test_controller, r_pin, gnd, vcc);
   
    rtos::run();
}