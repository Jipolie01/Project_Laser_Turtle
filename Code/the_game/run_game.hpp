#include "hwlib.hpp"
#include "rtos.hpp"
#include "display_classes.hpp"
#include "register_game_parameters_class.hpp"
#include "keypad_class.hpp"
#include "game_time_classes.hpp"

#ifndef RUN_GAME_HPP
#define RUN_GAME_HPP

class run_game_controller : public rtos::task<>{
private:
    lcd_display_controller & lcd_controller;
    rtos::mutex & lcd_mutex;
    
    hwlib::target::pin_oc out0 = hwlib::target::pin_oc( hwlib::target::pins::a0 );
    hwlib::target::pin_oc out1 = hwlib::target::pin_oc( hwlib::target::pins::a1 );
    hwlib::target::pin_oc out2 = hwlib::target::pin_oc( hwlib::target::pins::a2 );
    hwlib::target::pin_oc out3 = hwlib::target::pin_oc( hwlib::target::pins::a3 );
    hwlib::port_oc_from_pins out_port = hwlib::port_oc_from_pins( out0, out1, out2, out3 );
    hwlib::target::pin_in in0 = hwlib::target::pin_in( hwlib::target::pins::a4 );
    hwlib::target::pin_in in1 = hwlib::target::pin_in( hwlib::target::pins::a5 );
    hwlib::target::pin_in in2 = hwlib::target::pin_in( hwlib::target::pins::a6 );
    hwlib::target::pin_in in3 = hwlib::target::pin_in( hwlib::target::pins::a7 );
    hwlib::port_in_from_pins in_port = hwlib::port_in_from_pins( in0, in1, in2, in3 );
    
    
    ir_message_logic & message_logic;
    my_player_information & player_information;
    Keypad keypad;
    register_game_parameters_controller get_game_parameters;
    
    //rtos::flag game_time_over_flag;
    game_time_controller game_time;
    
    void main(void){
        
        //wait fire button
        //wait(button_name);
        
        while(1){
            
        }
    }
public:
    run_game_controller(lcd_display_controller & lcd_controller, rtos::mutex & lcd_mutex, ir_message_logic & message_logic, my_player_information & player_information):
        lcd_controller(lcd_controller),
        lcd_mutex(lcd_mutex),
        message_logic(message_logic),
        player_information(player_information),
        keypad(out_port, in_port),
        get_game_parameters(message_logic, player_information, keypad)
    {
        //initilization
        bool register_game_parameters_done = 0;
        char key = 0;
        
        lcd_passthrough lcd_clear;
        lcd_mutex.wait();
        lcd_controller.write(lcd_clear);
        lcd_mutex.signal();
        lcd_controller.enable_flag();
        while(1){
            key = keypad.check_for_input();
            //put game parameters key in lcd struct
            lcd_mutex.wait();
            auto lcd_struct = lcd_controller.read();
            lcd_struct.line4[0] = key;
            lcd_struct.line4[1] = '\0';
            lcd_controller.write(lcd_struct);
            lcd_mutex.signal();
            lcd_controller.enable_flag();
            
            register_game_parameters_done = get_game_parameters.setup(key);
            hwlib::wait_ms(200);
            if(register_game_parameters_done == 1){
                break;
            }
        }
        lcd_mutex.wait();
        lcd_controller.write(lcd_clear);
        lcd_mutex.signal();
        lcd_controller.enable_flag();
        
        //wait for time
        //wait(ir_receiver);
        
        game_time("game_time", 60, lcd_mutex, lcd_controller);
    }
    
    /*void enable_flag(){
        game_time_over_flag.set();
    }*/
};

#endif //RUN_GAME_HPP