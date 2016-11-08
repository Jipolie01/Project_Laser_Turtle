#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef DISPLAY_CLASSES_HPP
#define DISPLAY_CLASSES_HPP

struct lcd_passthrough{ //better name
    char line1[16] = "";//lives
    char line2[16] = "";//time left
    char line3[16] = "";//hit line
    char line4[16] = "";//commands
    char line5[16] = "";//some information
    char line6[16] = "";//some information
    char line7[16] = "";//some information
    char line8[16] = "";//some information
    bool player_hit = false;
    bool game_end = false;
    
    void assignment(char * array, const char * other){
        int i = 0;
        while(other[i] != '\0'){
            array[i] = other[i];
            i++;
        }
        array[i] = '\0';
    }
};

class lcd_display {
private:
    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
    hwlib::glcd_oled_buffered oled;
    hwlib::window_part window;
public:
    lcd_display(hwlib::pin_oc & scl, hwlib::pin_oc & sda):
        i2c_bus(scl, sda),
        oled(i2c_bus, 0x3c),
        window(oled, hwlib::location(0,0), hwlib::location(128, 64))
    {
        oled.clear();
    }
    
    void print_text(const char * string, hwlib::font & f){
        oled.clear();
        hwlib::window_ostream cout_stream(window, f);
        cout_stream << '\f' << string;
        oled.flush();
    }

};

class lcd_display_controller : public rtos::task<> {//if error check if this is correct
private:
    rtos::flag lcd_information_changed_flag;
    hwlib::font_default_8x8 default_font;
    hwlib::font_default_16x16 alternative_font;
    lcd_display oled;
    rtos::mutex & lcd_mutex;
    rtos::pool<lcd_passthrough> lcd_pool;
    
    char text_to_print[136];
    
    int compile(char *other, int start_pos){
        int i = 0;
        while(other[i] != '\0'){
            text_to_print[start_pos] = other[i];
            start_pos++;
            i++;
        }
        text_to_print[start_pos] = '\n';
        start_pos++;
        return start_pos;
    }
    
    void main(void){
        while(1){
            wait(lcd_information_changed_flag);
            lcd_mutex.wait();
            auto lcd_struct = lcd_pool.read();
            lcd_mutex.signal();
            
            if(lcd_struct.game_end){
                oled.print_text("Game\nover!!", alternative_font);
            }
            
            if(lcd_struct.player_hit){
                oled.print_text("\n\nHit!!", alternative_font);
                lcd_struct.player_hit = false;
                lcd_mutex.wait();
                write(lcd_struct);
                lcd_mutex.signal();
            }
            else{
                auto next_start = compile(lcd_struct.line1, 0);
                next_start = compile(lcd_struct.line2, next_start);
                next_start = compile(lcd_struct.line3, next_start);
                next_start = compile(lcd_struct.line4, next_start);
                next_start = compile(lcd_struct.line5, next_start);
                next_start = compile(lcd_struct.line6, next_start);
                next_start = compile(lcd_struct.line7, next_start);
                next_start = compile(lcd_struct.line8, next_start);
                text_to_print[next_start] = '\0';
                oled.print_text(text_to_print, default_font);
            }
        }
    }
public:
    lcd_display_controller(const char * task_name, hwlib::pin_oc & scl, hwlib::pin_oc & sda, rtos::mutex & lcd_mutex):
        task(task_name),
        lcd_information_changed_flag(this, "lcd_information_changed_flag"),
        oled(scl, sda),
        lcd_mutex(lcd_mutex),
        lcd_pool("lcd_pool")
    {}
    
    ~lcd_display_controller();
    
    
    
    //functions for synchronization mechanisms
    void write(const lcd_passthrough lcd_struct){
        lcd_pool.write(lcd_struct);
    }
    
     lcd_passthrough read(){
        return lcd_pool.read();
    }
    
    void enable_flag(){
        lcd_information_changed_flag.set();
    }
};

#endif // DISPLAY_CLASSES_HPP
