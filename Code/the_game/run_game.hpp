#include "hwlib.hpp"
#include "rtos.hpp"
#include "display_classes.hpp"
#include "register_game_parameters_class.hpp"
#include "keypad_class.hpp"
#include "game_time_classes.hpp"
#include "listenerpattern.hpp"
#include "speaker_classes.hpp"
#include "rgb_led_classes.hpp"

#ifndef RUN_GAME_HPP
#define RUN_GAME_HPP
//wouter vragen
//waarom de fuck als ik een volledig andere class toevoeg gaat hij zeggen dat hij lcd niet meer kent
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
    rtos::flag game_time_flag;
    game_time_controller game_time;
    
    rtos::flag button_pressed_flag;
    hwlib::target::pin_in button_pin = hwlib::target::pin_in(hwlib::target::pins::d12);
    button_controller button_ctrl;
    
    hwlib::target::pin_out speaker_pin = hwlib::target::pin_out(hwlib::target::pins::d7);
    rtos::mutex & sound_mutex;
    speaker_controller speaker_ctrl;
    
    hwlib::target::pin_out pin_r = hwlib::target::pin_out(hwlib::target::pins::d6);
    hwlib::target::pin_out pin_g = hwlib::target::pin_out(hwlib::target::pins::d5);
    hwlib::target::pin_out pin_b = hwlib::target::pin_out(hwlib::target::pins::d4);
    led_controller led_ctrl;
    
    void main(void){
        //initilization
        bool register_game_parameters_done = 0;
        char key = 0;
        
        lcd_passthrough lcd_commands;
        lcd_commands.assignment(lcd_commands.line3, "Type command");
        lcd_mutex.wait();
        lcd_controller.write(lcd_commands);
        lcd_mutex.signal();
        lcd_controller.enable_flag();
        sleep(100*rtos::ms);
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
            sleep(200 *rtos::ms);
            if(register_game_parameters_done == 1){
                break;
            }
        }
        lcd_passthrough lcd_clear;
        lcd_mutex.wait();
        lcd_controller.write(lcd_clear);
        lcd_mutex.signal();
        lcd_controller.enable_flag();
        sleep(100*rtos::ms);
        
        //suspend all tasks
        /*game_time.suspend();
        button_ctrl.suspend();
        lcd_controller.suspend();*/
        //wait for time
        //wait(ir_receiver);
        //do receive stuf
        
        /*game_time.suspend();
        button_ctrl.resume();//might need to be only done for periodic tasks
        lcd_controller.resume();*/
        int cooldown_time = 10;
        int play_time = 40;
        game_time.set_time(cooldown_time, play_time); //change to some received time
        led_color_behaviour rgb_led_struct;
        
        rgb_led_struct.red = true;
        rgb_led_struct.green = false;
        rgb_led_struct.blue = false;
        led_ctrl.write(rgb_led_struct);
        sleep(100 * rtos::ms);
        
        //wait fire button
        wait(button_pressed_flag);
        sleep(1000*rtos::ms);
        game_time.enable_start_flag();
        sleep( (cooldown_time+(cooldown_time/2)) * 1000 * rtos::ms);
        
        rgb_led_struct.red = false;
        rgb_led_struct.green = true;
        rgb_led_struct.blue = false;
        led_ctrl.write(rgb_led_struct);
        hwlib::cout << "na rgb led\n";
        sleep(700*rtos::ms);
        
        //update health
        lcd_passthrough new_struct;
        new_struct.assignment(new_struct.line1, "Health: 100");
        lcd_mutex.wait();
        lcd_controller.write(new_struct);
        lcd_mutex.signal();
        lcd_controller.enable_flag();
        sleep(900 * rtos::ms);
        game_time.enable_start_flag();
        sleep(1000*rtos::ms);
        
        while(1){
            auto event = wait(button_pressed_flag + game_time_flag);
            hwlib::cout << "uit";
            if(event == game_time_flag){
                game_time.suspend();
                button_ctrl.suspend();
                lcd_controller.suspend();
                speaker_ctrl.suspend();
                led_ctrl.suspend();
                hwlib::cout << "suspended\n";
            }
            else if(event == button_pressed_flag){
                hwlib::cout << "shoot\n";
                rgb_led_struct.green = false;
                rgb_led_struct.red = false;
                rgb_led_struct.blue = true;
                led_ctrl.write(rgb_led_struct);
                hwlib::cout << "na led\n";
                sleep(1000*rtos::ms);
                sound_mutex.wait();
                speaker_ctrl.write(speaker_ctrl.shoot);
                sound_mutex.signal();
                speaker_ctrl.enable_flag();
                hwlib::cout << "na speaker\n";
                sleep(700 * rtos::ms);
            }/* //add * instead of & to button
            else if(event == received_information_channel){
                rgb_led_struct.green = false;
                rgb_led_struct.red = true;
                rgb_led_struct.blue = false;
                rgb_led_struct.times_to_blink = 0;
                rgb_led_struct.delay_ms = 0;
                led_ctrl.write(newstruct);
                
                sound_mutex.wait();
                sound_ctrl.write(sound_ctrl.hit);
                sound_mutex.signal();
                sound_ctrl.enable_flag();
                // sleep();
            }*/
        }
    }
public:
    run_game_controller(lcd_display_controller & lcd_controller, rtos::mutex & lcd_mutex, ir_message_logic & message_logic, my_player_information & player_information)://, rtos::mutex & sound_mutex):
        lcd_controller(lcd_controller),
        lcd_mutex(lcd_mutex),
        message_logic(message_logic),
        player_information(player_information),
        keypad(out_port, in_port),
        get_game_parameters(message_logic, player_information, keypad),
        game_time_flag(this, "game_time_flag"),
        game_time("game_time", lcd_mutex, lcd_controller, game_time_flag),
        button_pressed_flag(this, "button_pressed_flag"),
        button_ctrl(button_pin, button_pressed_flag),
        sound_mutex(sound_mutex),
        speaker_ctrl("speaker_ctrl", speaker_pin, sound_mutex),
        led_ctrl("led_controller", pin_r, pin_g, pin_b)
    {}
};

#endif //RUN_GAME_HPP