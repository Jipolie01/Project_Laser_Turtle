/// @file
#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef DISPLAY_CLASSES_HPP
#define DISPLAY_CLASSES_HPP

///Lcd line structure
//
///This structure is used to make sure we can edit individual lines on the lcd.
///It contains 8 lines of 16 characters long. There are also some boolean values
/// for making it easy to tell the controller you want one of three bigger texts
/// on the lcd. These texts are "hit", "game begin" and "game end"
struct lcd_passthrough{ //better name
    char line1[16] = "";//lives
    char line2[16] = "";//time left
    char line3[16] = "";//hit line
    char line4[16] = "";//some information
    char line5[16] = "";//commands
    char line6[16] = "";//some information
    char line7[16] = "";//some information
    char line8[16] = "";//some information
    bool player_hit = false;
    bool game_end = false;
    bool game_begin = false;
    
    ///Assign character array to line
    //
    ///This function is used to assign a character array to the specified line.
    /// It makes sure that two different sized character arrays can be used.
    ///The function dus not have a return value but requires two char arrays
    /// as parameters.
    void assignment(char * array, const char * other){
        int i = 0;
        while(other[i] != '\0'){
            array[i] = other[i];
            i++;
        }
        array[i] = '\0';
    }
};

///display class
//
///This is the display boundary object we use for interfacing with the lcd.
///It creates an lcd using a i2c bus.
class lcd_display {
private:
    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
    hwlib::glcd_oled_buffered oled;
    hwlib::window_part window;
public:
    ///Default constructor
    //
    ///This constructor receives the scl and sda lines as parameters.
    ///It then initializes them as an i2c bus. The rest of the variables
    /// and objects are also initialized and the lcd is beeing cleared.
    lcd_display(hwlib::pin_oc & scl, hwlib::pin_oc & sda):
        i2c_bus(scl, sda),
        oled(i2c_bus, 0x3c),
        window(oled, hwlib::location(0,0), hwlib::location(128, 64))
    {
        oled.clear();
    }
    
    ///Print text on display
    //
    ///This function prints a character array on the lcd with a specified font.
    ///It receives both these variables as parameters. There is no return value.
    void print_text(const char * string, hwlib::font & f){
        oled.clear();
        hwlib::window_ostream cout_stream(window, f);
        cout_stream << '\f' << string;
        oled.flush();
    }

};

///Display controller
//
///The display controller is used to get information to be printed from
/// a pool with mutex and then output it on the lcd.
class lcd_display_controller : public rtos::task<> {//if error check if this is correct
private:
    rtos::flag lcd_information_changed_flag;
    hwlib::font_default_8x8 default_font;
    hwlib::font_default_16x16 alternative_font;
    lcd_display oled;
    rtos::mutex & lcd_mutex;
    rtos::pool<lcd_passthrough> lcd_pool;
    
    char text_to_print[136];
    
    ///Compile line
    //
    ///This function receives a line and a start position from the main
    /// and compiles it. This means the characters are beeing put into the 
    /// text to print array beginning at the start position. The function
    /// returns the integer of the next start position.
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
    
    ///main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. First it waits on the information
    /// changed flag to point out that new information has
    /// been put into the lcd pool. After that it reads the pool
    /// and checks for certain booleans to be set. If none are set
    /// the function compiles the lines located in the structure
    /// and outputs them on the screen.
    void main(void){
        while(1){
            wait(lcd_information_changed_flag);
            lcd_mutex.wait();
            auto lcd_struct = lcd_pool.read();
            lcd_mutex.signal();
            
            if(lcd_struct.game_end){
                oled.print_text("Game\nover!!", alternative_font);
            }
            else if(lcd_struct.game_begin){
                oled.print_text("Game\nstart", alternative_font);
            }
            
            else if(lcd_struct.player_hit){
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
    ///Default constructor
    //
    ///This constructor receives a task name, scl pin, sda pin, and lcd mutex
    /// as parameters. It initializes these variables and creates the oled
    /// object from them. It also initializes the information changed flag.
    lcd_display_controller(const char * task_name, hwlib::pin_oc & scl, hwlib::pin_oc & sda, rtos::mutex & lcd_mutex):
        task(7, task_name),
        lcd_information_changed_flag(this, "lcd_information_changed_flag"),
        oled(scl, sda),
        lcd_mutex(lcd_mutex),
        lcd_pool("lcd_pool")
    {}
    
    
    ///Write to pool
    //
    ///This function makes other classes able to write lcd passthrough
    /// structures to the lcd pool. It returns nothing and receives the
    /// structure to write as an parameter.
    void write(const lcd_passthrough lcd_struct){
        lcd_pool.write(lcd_struct);
    }
    
    ///Read from pool
    //
    ///This function is used to read the current data from the pool.
    ///This makes other objects able to read the current data displayed
    /// on the lcd and change a single line while maintaining the rest
    /// of the data. It returns the information from the pool and dus
    /// not need any parameters.
    lcd_passthrough read(){
        return lcd_pool.read();
    }
    
    ///Enable the lcd information changed flag
    //
    ///This function is used to set the information changed flag so
    /// the main knows that new information has to be put on screen.
    ///The function dus not need any parameters nor dus it return anything.
    void enable_flag(){
        lcd_information_changed_flag.set();
    }
};

#endif // DISPLAY_CLASSES_HPP
