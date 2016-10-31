#include "hwlib.hpp"

#ifndef DISPLAY_CLASSES_HPP
#define DISPLAY_CLASSES_HPP

class lcd_display {
private:
    hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
    hwlib::glcd_oled_buffered oled;
    hwlib::window_part window;
    //hwlib::window_ostream cout_stream_hit;
public:
    lcd_display(hwlib::pin_oc & scl, hwlib::pin_oc & sda):
        i2c_bus(scl, sda),
        oled(i2c_bus, 0x3c),
        window(oled, hwlib::location(0,0), hwlib::location(128, 64))
    {
        oled.clear();
    }
    
    
    ~lcd_display();
    
    void print_text(const char * string, hwlib::font & f){
        oled.clear();
        hwlib::window_ostream cout_stream(window, f);
        cout_stream << '\f' << string;
        oled.flush();
    }

};

#endif // DISPLAY_CLASSES_HPP
