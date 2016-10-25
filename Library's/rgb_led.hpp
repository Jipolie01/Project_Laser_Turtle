#ifndef RGB_LED_HPP
#define RGB_LED_HPP

#include "hwlib.hpp"

class rgb_led {
private:
    hwlib::pin_out & r;
    hwlib::pin_out & g;
    hwlib::pin_out & b;
public:
    rgb_led(hwlib::pin_out & r, hwlib::pin_out & g,hwlib::pin_out & b):
        r(r),
        g(g),
        b(b)
    {}
    
    void set_red(){
        r.set(1);
        g.set(0);
        b.set(0);
    }
    
    void set_green(){
        r.set(0);
        g.set(1);
        b.set(0);
    }
    
    void set_blue(){
        r.set(0);
        g.set(0);
        b.set(1);
    }
    
    void set_brown(){
        r.set(1);
        g.set(1);
        b.set(0);
    }
    
    void set_light_blue(){
        r.set(0);
        g.set(1);
        b.set(1);
    }
    
    void set_purple(){
        r.set(1);
        g.set(0);
        b.set(1);
    }
    
    void set_white(){
        r.set(1);
        g.set(1);
        b.set(1);
    }
    
    void off(){
        r.set(0);
        g.set(0);
        b.set(0);
    }
};

#endif // RGB_LED_HPP
