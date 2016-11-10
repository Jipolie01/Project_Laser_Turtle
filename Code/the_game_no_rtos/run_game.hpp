/// @file
#include "hwlib.hpp"
#include "rtos.hpp"
#include "display_classes.hpp"
#include "register_game_parameters_class.hpp"
#include "keypad_class.hpp"
#include "game_time_classes.hpp"
#include "listenerpattern.hpp"
#include "speaker_classes.hpp"
#include "rgb_led_classes.hpp"
#include "send_ir_classes.hpp"

#ifndef RUN_GAME_HPP
#define RUN_GAME_HPP

///run the game
//
///This class is the total package of the game. Most of the classes
/// used are created in this class. It coordinates almost all the
/// information flows.
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
    rtos::flag game_over_flag;
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
    
    send_controller sender;
    
    rtos::channel<char16_t, 10> received_information_channel;
    int hits_array_pos = 0;
    hit * hits[10];
    byte player_id = 0;
    byte weapon_id = 0;
    char health_array[16] = "Health: 100";
    
    ///main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. It starts by initializing the game
    /// parameters. After that it waits for the game to be started
    /// by the press of the "trigger" button. The next thing it
    /// does is waiting for an event from the "trigger" button
    /// , the game time or the ir receiver. It handles each of
    /// these events differently.
    void main(void){
        //initilization
        /*bool register_game_parameters_done = 0;
        char key = 0;
        
        lcd_passthrough lcd_commands;
        lcd_commands.assignment(lcd_commands.line3, "Type command");
        lcd_mutex.wait();
        lcd_controller.write(lcd_commands);
        lcd_mutex.signal();
        lcd_controller.enable_flag();
        sleep(100*rtos::ms);
        while(1){
            key = keypad.get_char();
            
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
        }*/
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
        int play_time = 0;
        //wait for time
        while(1){
            player_id = 0;
            weapon_id = 0;
            wait(received_information_channel);
            //check for start bit
            auto bitstream = received_information_channel.read();
            if(bitstream != -1){
                if(message_logic.decode(bitstream, player_id, weapon_id)){
                    if(player_id == 0){
                        play_time = weapon_id;
                        hwlib::cout << (int)weapon_id << '\n';
                        break;
                    }
                    else{
                        continue;
                    }
                }
            }
            else{
                continue;
            }
        }
        
        /*game_time.suspend();
        button_ctrl.resume();//might need to be only done for periodic tasks
        lcd_controller.resume();*/
        int cooldown_time = 10;
        
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
        sleep(700*rtos::ms);
        
        //update health
        lcd_passthrough new_struct;
        new_struct.assignment(new_struct.line1, health_array);
        lcd_mutex.wait();
        lcd_controller.write(new_struct);
        lcd_mutex.signal();
        lcd_controller.enable_flag();
        sleep(900 * rtos::ms);
        game_time.enable_start_flag();
        sleep(1000*rtos::ms);
        
        while(1){
            rgb_led_struct.red = false;
            rgb_led_struct.green = true;
            rgb_led_struct.blue = false;
            led_ctrl.write(rgb_led_struct);
            
            auto event = wait(button_pressed_flag + game_over_flag);// + received_information_channel);
            
            if(event == game_over_flag){
                rgb_led_struct.red = true;
                rgb_led_struct.green = false;
                rgb_led_struct.blue = false;
                led_ctrl.write(rgb_led_struct);
                sleep(1500*rtos::ms);
                lcd_controller.suspend();
                game_time.suspend();
                button_ctrl.suspend();
                speaker_ctrl.suspend();
                led_ctrl.suspend();
                sender.suspend();
                //receiver.suspend();
                hwlib::cout << "suspended\n";
                while(1){}
            }
            else if(event == button_pressed_flag){
                button_ctrl.suspend();
                
                sender.enable_flag();
                rgb_led_struct.green = false;
                rgb_led_struct.red = false;
                rgb_led_struct.blue = true;
                led_ctrl.write(rgb_led_struct);
                
                sound_mutex.wait();
                speaker_ctrl.write(speaker_ctrl.shoot);
                sound_mutex.signal();
                speaker_ctrl.enable_flag();
                sleep(100 * rtos::ms);
                button_ctrl.resume();
            }/*
            else if(event == received_information_channel){
                auto information = received_information_channel.read();
                if(message_logic.decode(information, player_id, weapon_id)){
                    hit h(player_id, weapon_id);
                    hits[hits_array_pos] = &h;
                    hits_array_pos++;
                    rgb_led_struct.green = false;
                    rgb_led_struct.red = true;
                    rgb_led_struct.blue = false;
                    led_ctrl.write(newstruct);
                    
                    //update lcd with hit
                    lcd_passthrough tmp;
                    tmp.player_hit = true;
                    lcd_mutex.wait();
                    auto lcd_struct = lcd_controller.read();
                    lcd_controller.write(tmp);
                    lcd_mutex.signal();
                    
                    sound_mutex.wait();
                    speaker_ctrl.write(speaker_ctrl.hit);
                    sound_mutex.signal();
                    speaker_ctrl.enable_flag();
                    sleep(100*rtos::ms);
                    
                    player_information.set_health(player_information.get_health() - 10);
                    auto health = player_information.get_health();
                    
                    if(health > 0){
                        convertInt(health, healt_array, 8,9);
                        healt_array[10] = '\0';
                    }
                    else{
                        lcd_struct.game_end = true;
                    }
                    lcd_mutex.wait();
                    lcd_controller.write(lcd_struct);
                    lcd_mutex.signal();
                    sleep(1500*rtos::ms);
                    if(health == 0){
                        lcd_controller.suspend();
                        game_time.suspend();
                        button_ctrl.suspend();
                        speaker_ctrl.suspend();
                        led_ctrl.suspend();
                        sender.suspend();
                        //receiver.suspend();
                        while(1){}
                    }
                }
            }*/
        }
    }
    
    
    ///convert int to char
    //
    ///This function is used to convert an integer to a character
    /// and put it in an array. The parameters are the number that
    /// needs to be converted, a character array for the output
    /// and an begin and end for where the characters need to be put
    /// into the array. This function originally was created by
    /// someone else. We just changed it to serve our needs.
    ///original link: http://www.cplusplus.com/forum/beginner/7777/
    ///The function dus not have a return type but does return the
    /// characters through the by pointer received array (in the
    /// parameter list).
    void convert_int(int number, char * array, int begin, int end){
        if (number == 0){
            array[begin] = '0';
            array[end] = '0';
            return;
        }
        else if(number < 10){
            array[begin] = '0';
            begin++;
        }
        char temp[5] ="";
        int amount = 0;
        while (number>0){
            temp[amount]=number%10+48;
            number/=10;
            amount++;
        }
        temp[amount] = '\0';
        
        int i = 0;
        while(temp[i] != '\0'){
            array[i+begin] = temp[amount - (i+1)];
            i++;
        }
    }
public:
    
    ///Default constructor
    //
    ///The constructor of this class initializes a lot of objects
    /// needed for the object to work. The parameters are all the
    /// parts that are created outside the class. This consists of
    /// the lcd controller object, the lcd mutex, the message
    /// application logic object, the player information entity and
    /// the sound mutex.
    run_game_controller(lcd_display_controller & lcd_controller, rtos::mutex & lcd_mutex, ir_message_logic & message_logic, my_player_information & player_information, rtos::mutex & sound_mutex):
        task(6, "run_game"),
        lcd_controller(lcd_controller),
        lcd_mutex(lcd_mutex),
        message_logic(message_logic),
        player_information(player_information),
        keypad(out_port, in_port),
        get_game_parameters(message_logic, player_information, keypad),
        game_over_flag(this, "game_over_flag"),
        game_time("game_time", lcd_mutex, lcd_controller, game_over_flag),
        button_pressed_flag(this, "button_pressed_flag"),
        button_ctrl(button_pin, this),
        sound_mutex(sound_mutex),
        speaker_ctrl("speaker_ctrl", speaker_pin, sound_mutex),
        led_ctrl("led_controller", pin_r, pin_g, pin_b),
        sender(player_information),
        received_information_channel(this, "received_information_channel")
    {}
    
    ///Enable the button flag
    //
    ///This function gives other objects the ability so set the
    /// button pressed flag.
    void enable_flag(){
        button_pressed_flag.set();
    }
    
    void write(char16_t value){
        received_information_channel.write(value);
    }
};

#endif //RUN_GAME_HPP