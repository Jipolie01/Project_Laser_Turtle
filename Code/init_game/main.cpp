#include "hwlib.hpp"
#include "rtos.hpp"
#include "init_game.hpp"
#include "display_classes.hpp"

int main(void){
    //Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    namespace target = hwlib::target;
    
    auto scl = target::pin_oc{ target::pins::d21};
    auto sda = target::pin_oc{ target::pins::d20};
    
    ir_message_logic message;
    
    auto lcd_mutex = rtos::mutex("lcd_mutex");
    auto lcd_controller = lcd_display_controller("lcd_display_controller", scl, sda, lcd_mutex);
    
    auto init_game = init_game_controller(lcd_controller, lcd_mutex, message);
    rtos::run();
}