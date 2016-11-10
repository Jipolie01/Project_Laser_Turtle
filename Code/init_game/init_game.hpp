/// @file
#include "rtos.hpp"
#include "hwlib.hpp"
#include "keypad_class.hpp"
#include "display_classes.hpp"
#include "application_logic_classes.hpp"
#include "send_ir_classes.hpp"

#ifndef INIT_GAME_HPP
#define INIT_GAME_HPP

///Inialize the game
//
///This class is used for the game leader to setup the game.
///So far the only command the receiver expects is the game
/// time at the beginning of the game. New commands can be
/// added later and would need a change in the run game class.
///Right now during the game the receiver only exprects player
/// ids and weapon ids but no commands.
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
    
    ///Main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. The function waits for commands and
    /// sends them using the # button. # can be pressed multiple
    /// times to send the command multiple times. * can be pressed
    /// to indicate that you are done with sending commands.
    void main(void){
        char key = 0;
        uint_fast8_t to_send;
        int returned = 1;
        lcd_passthrough lcd_commands;
        while(1){
            if(returned){
                lcd_commands.assignment(lcd_commands.line3, "Type C to");
                lcd_commands.assignment(lcd_commands.line4, "initialize");
                lcd_mutex.wait();
                lcd_controller.write(lcd_commands);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
                sleep(100*rtos::ms);
                returned = 0;
            }
            if(key == 'C'){
                hwlib::cout << key;
                to_send = 0;
                
                lcd_commands.assignment(lcd_commands.line3, "Type command");
                lcd_commands.assignment(lcd_commands.line4, "C");
                lcd_mutex.wait();
                lcd_controller.write(lcd_commands);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
                sleep(100*rtos::ms);
                int i = 0;
                while(1){
                    key = keypad.get_char();
                    
                    if((key >= '0') && (key <= '9')){
                        hwlib::cout << key;
                        lcd_mutex.wait();
                        auto lcd_struct = lcd_controller.read();
                        lcd_struct.line4[i] = key;
                        lcd_struct.line4[(i+1)] = '\0';
                        i++;
                        lcd_controller.write(lcd_struct);
                        lcd_mutex.signal();
                        lcd_controller.enable_flag();
                        sleep(100*rtos::ms);
                        to_send = (to_send*10)+(key-'0');
                        hwlib::cout << '\n' << to_send << '\n';
                    }
                    if(key == '#'){
                        hwlib::cout << key;
                        lcd_mutex.wait();
                        auto lcd_struct = lcd_controller.read();
                        lcd_struct.line4[i] = key;
                        lcd_struct.line4[(i+1)] = '\0';
                        i++;
                        lcd_controller.write(lcd_struct);
                        lcd_mutex.signal();
                        lcd_controller.enable_flag();
                        sleep(100*rtos::ms);
                        char16_t encoded_message = message.encode(0, to_send);
                        sender.write(encoded_message);
                    }
                    if(key == '*'){
                        returned = 1;
                        break;
                    }
                }
            }
            key = keypad.get_char();
        }
    }
public:
    ///Default constructor
    //
    ///This constructor receives a reference to the lcd controller 
    /// and the lcd mutex. These are used to communicate with the
    /// lcds pool. The message object is there for encoding every
    /// seperate command.
    init_game_controller(lcd_display_controller & lcd_controller, rtos::mutex & lcd_mutex, ir_message_logic & message):
        task(3, "init_game"),
        lcd_controller(lcd_controller),
        lcd_mutex(lcd_mutex),
        message(message),
        keypad(out_port, in_port)
    {}
};

#endif //INIT_GAME_HPP