#include "hwlib.hpp"
#include "rtos.hpp"
#include "application_logic_classes.hpp"
#include "entity_classes.hpp"
#include "run_game.hpp"
#include "display_classes.hpp"

int main(void){
    //Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    namespace target = hwlib::target;
    
    auto scl = target::pin_oc{ target::pins::d21};
    auto sda = target::pin_oc{ target::pins::d20};
    
    ir_message_logic message_logic;
    my_player_information player_information;
    
    auto lcd_mutex = rtos::mutex("lcd_mutex");
    auto lcd_controller = lcd_display_controller("lcd_display_controller", scl, sda, lcd_mutex);
    
    auto sound_mutex = rtos::mutex("sound_mutex");
    auto run_game = run_game_controller(lcd_controller, lcd_mutex, message_logic, player_information, sound_mutex);
    
    //auto receiver = receiverding(run_game);
    rtos::run();
}
