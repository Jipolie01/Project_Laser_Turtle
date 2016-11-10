#include "rtos.hpp"
#include "hwlib.hpp"
#include "keypad_class.hpp"
#include "display_classes.hpp"
#include "application_logic_classes.hpp"
#include "send_ir_classes.hpp"

#ifndef INIT_GAME_HPP
#define INIT_GAME_HPP

class init_game_controller : public rtos::task<>{
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
    
    ir_message_logic & message;
    Keypad keypad;
    send_controller sender;
    
    void main(void){
        char key = 0;
        int to_send;
        lcd_passthrough lcd_commands;
        lcd_passthrough lcd_clear;
        while(1){
            if(key == 'C'){
                hwlib::cout << key;
                to_send = 0;
                lcd_mutex.wait();
                lcd_controller.write(lcd_clear);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
                sleep(100*rtos::ms);
                
                lcd_commands.assignment(lcd_commands.line3, "Type command");
                lcd_mutex.wait();
                lcd_controller.write(lcd_commands);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
                sleep(100*rtos::ms);
                
                while(1){
                    key = keypad.check_for_input();
                    
                    lcd_mutex.wait();
                    auto lcd_struct = lcd_controller.read();
                    lcd_struct.line4[0] = key;
                    lcd_struct.line4[1] = '\0';
                    lcd_controller.write(lcd_struct);
                    lcd_mutex.signal();
                    lcd_controller.enable_flag();
                    sleep(100*rtos::ms);
                    
                    if((key >= '0') && (key <= '9')){
                        hwlib::cout << key;
                        to_send = (to_send*10)+key;
                    }
                    if(key == '#'){
                        hwlib::cout << key;
                        char16_t encoded_message = message.encode(0, to_send);
                        sender.write(encoded_message);
                        break;
                    }
                }
            }
            key = keypad.check_for_input();
        }
    }
public:
    init_game_controller(lcd_display_controller & lcd_controller, rtos::mutex & lcd_mutex, ir_message_logic & message):
        task(0, "init_game"),
        lcd_controller(lcd_controller),
        lcd_mutex(lcd_mutex),
        message(message),
        keypad(out_port, in_port)
    {}
};

#endif //INIT_GAME_HPP