#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef RGB_LED_CLASSES_HPP
#define RGB_LED_CLASSES_HPP



struct led_color_behaviour{
    bool red = false;
    bool green = false;
    bool blue = false;
   // unsigned int delay_ms = 0;
   // unsigned int times_to_blink = 0;
};

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
    
    /*
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
    }*/
    
    void off(){
        r.set(0);
        g.set(0);
        b.set(0);
    }
};

class led_controller : public rtos::task<> {//if error check if this is correct
private:
    rtos::channel<led_color_behaviour, 10> led_color_channel;
    rgb_led led;
    
    void set_color(led_color_behaviour & color_struct){
        if(color_struct.red){
                led.set_red();
        }
        else if(color_struct.green){
                led.set_green();
        }
        else if(color_struct.blue){
                led.set_blue();
        }
    }
    
    void main(void){
        while(1){
            wait(led_color_channel);
            auto color_struct = led_color_channel.read();
            /*if(color_struct.times_to_blink != 0){
                for(unsigned int i = 0; i < color_struct.times_to_blink; i++){
                    set_color(color_struct);
                    sleep(color_struct.delay_ms * rtos::ms);
                    led.off();
                    sleep(color_struct.delay_ms * rtos::ms);
                    hwlib::cout << i << '\n';
                }
            }
            else{*/
            set_color(color_struct);
            //}
        }
    }
public:
    led_controller(const char * task_name, hwlib::pin_out & r, hwlib::pin_out & g, hwlib::pin_out & b):
        task(task_name),
        led_color_channel(this, "led_color_channel"),
        led(r, g, b)
    {}
    

    //functions for synchronization mechanism
    /*led_color_behaviour read(){
        return led_color_channel.read();
    }*/
    
    void write(led_color_behaviour color){
        led_color_channel.write(color);
    }
};

#endif // RGB_LED_CLASSES_HPP
