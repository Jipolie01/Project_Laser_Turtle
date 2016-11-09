#include "hwlib.hpp"
#include "rtos.hpp"
#include "entity_classes.hpp"
#include "display_classes.hpp"

#ifndef GAME_TIME_CLASSES_HPP
#define GAME_TIME_CLASSES_HPP


class game_time_controller : public rtos::task<> {//if error check if this is correct
private:
    rtos::clock game_time_clock;
    rtos::flag game_start_flag;
    game_information_data time_data;
    rtos::mutex & lcd_mutex;
    lcd_display_controller & lcd_controller;
    rtos::flag & game_time_flag;
    //run_game_controller & run_game;
    
    //http://www.cplusplus.com/forum/beginner/7777/
    void convertInt(int number, char * array, int begin, int end){
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
    
    void main(void){
        
        wait(game_start_flag);
        time_data.set_game_has_start(true);
        
        while(1){
            wait(game_time_clock);
            time_data.set_cooldown_time((time_data.get_cooldown_time()-1));
            
            int new_time = time_data.get_cooldown_time();
            if(new_time <= 0){
                lcd_passthrough new_struct;
                //new_struct.game_end = true;
                new_struct.game_begin = true;
                lcd_mutex.wait();
                lcd_controller.write(new_struct);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
                break;
            }
            else{
                char new_array[16] = "";
                convertInt((new_time/60), new_array, 0, 1);
                new_array[2] = ':';
                convertInt((new_time%60), new_array, 3, 4);
                new_array[5] = '\0';
                
                lcd_mutex.wait();
                auto lcd_struct = lcd_controller.read();
                lcd_struct.assignment(lcd_struct.line2, new_array);
                lcd_struct.assignment(lcd_struct.line1, "Cooldown time");
                lcd_controller.write(lcd_struct);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
            }
        }
        
        /*lcd_passthrough new_struct;
        lcd_mutex.wait();
        lcd_controller.write(new_struct);
        lcd_mutex.signal();
        lcd_controller.enable_flag();*/
        //game_time_flag.set();
        //run_game.enable_flag()
        hwlib::cout << "start_flag\n";
        wait(game_start_flag);
        hwlib::cout << "start_flag set\n";
        while(1){
            wait(game_time_clock);
            time_data.set_game_time((time_data.get_game_time()-1));
            
            int new_time = time_data.get_game_time();
            if(new_time <= 0){
                lcd_passthrough new_struct;
                new_struct.game_end = true;
                lcd_mutex.wait();
                lcd_controller.write(new_struct);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
                sleep(1000*rtos::ms);
                game_time_flag.set();
                //suspend this task (might need to be done in run game)
            }
            else{
                char new_array[16] = "Time: ";
                convertInt((new_time/60), new_array, 6, 7);
                new_array[8] = ':';
                convertInt((new_time%60), new_array, 9, 10);
                new_array[11] = '\0';
                
                lcd_mutex.wait();
                auto lcd_struct = lcd_controller.read();
                lcd_struct.assignment(lcd_struct.line2, new_array);
                lcd_controller.write(lcd_struct);
                lcd_mutex.signal();
                lcd_controller.enable_flag();
                sleep(1*rtos::ms);
            }
        }
    }
public:
    game_time_controller(const char * task_name, rtos::mutex & lcd_mutex, lcd_display_controller & lcd_controller, rtos::flag & game_time_flag)://, run_game_controller & run_game):
        task(1, task_name),
        game_time_clock(this, 1000*rtos::ms, "game_time_clock"),
        game_start_flag(this, "game_ended_flag"),
        lcd_mutex(lcd_mutex),
        lcd_controller(lcd_controller),
        game_time_flag(game_time_flag)
    {}
    
    void enable_start_flag(){
        game_start_flag.set();
    }
    
    void set_time(int cooldown_time, int game_time){
        time_data.set_cooldown_time(cooldown_time);
        time_data.set_game_time(game_time);
    }
};

#endif // GAME_TIME_CLASSES_HPP
