#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef RUN_GAME_HPP
#define RUN_GAME_HPP

class run_game_controller : public rtos::task<>{
private:
    lcd_controller & lcd;
    
    void main(void){
        //initilization
        //execute register game parameters
        
        while(1){
            
        }
    }
public:
    run_game_controller(lcd_controller & lcd):
        lcd(lcd)
    {}
}

#endif //RUN_GAME_HPP