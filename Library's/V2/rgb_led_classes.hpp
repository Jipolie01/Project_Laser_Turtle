/// @file
#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef RGB_LED_CLASSES_HPP
#define RGB_LED_CLASSES_HPP


///Provide led color information
//
///This struct is used to provide information about what color
/// needs to be visable on the led.
struct led_color_behaviour{
    bool red = false;
    bool green = false;
    bool blue = false;
};

///Rgb led
//
///Simple boundary object used to set an rgb led to one of the three colors.
class rgb_led {
private:
    hwlib::pin_out & r;
    hwlib::pin_out & g;
    hwlib::pin_out & b;
public:
    ///Default constructor
    //
    ///This constructor gets three pins as parameters and initializes them.
    rgb_led(hwlib::pin_out & r, hwlib::pin_out & g,hwlib::pin_out & b):
        r(r),
        g(g),
        b(b)
    {}
    
    ///turn the red color on
    //
    ///This function turns on the red color on the led
    /// while turing off the other colors. It dus not
    ///need any parameters nor dus it return anything.
    void set_red(){
        r.set(1);
        g.set(0);
        b.set(0);
    }
    
    ///turn the green color on
    //
    ///This function turns on the green color on the led
    /// while turing off the other colors. It dus not
    ///need any parameters nor dus it return anything.
    void set_green(){
        r.set(0);
        g.set(1);
        b.set(0);
    }
    
    ///turn the blue color on
    //
    ///This function turns on the blue color on the led
    /// while turing off the other colors. It dus not
    ///need any parameters nor dus it return anything.
    void set_blue(){
        r.set(0);
        g.set(0);
        b.set(1);
    }
    
    ///turn the led of
    //
    ///This function turns off all the led colors.
    ///It dus not need any parameters nor dus it
    ///return anything.
    void off(){
        r.set(0);
        g.set(0);
        b.set(0);
    }
};

///Led controller class
//
///This class controls the behaviour of the led. It receives
/// this behaviour through a rtos channel.
class led_controller : public rtos::task<> {//if error check if this is correct
private:
    rtos::channel<led_color_behaviour, 10> led_color_channel;
    rgb_led led;
    
    ///Set the color
    //
    ///This function sets the color to the specified color.
    ///It requires a led color behaviour struct as parameter
    /// and dus not return anything.
    void set_color(led_color_behaviour & color_struct){
        if(color_struct.red){
                led.off();
                led.set_red();
        }
        else if(color_struct.green){
                led.off();
                led.set_green();
        }
        else if(color_struct.blue){
                led.off();
                led.set_blue();
        }
    }
    
    ///main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. The function simply waits for the channel
    /// to receive something. Then reads it and sets the led to
    /// the correct color.
    void main(void){
        while(1){
            wait(led_color_channel);
            auto color_struct = led_color_channel.read();
            set_color(color_struct);
        }
    }
public:
    ///Default constructor
    //
    ///This constructor receives a task name and three pins.
    ///The pins from the led. The led object gets initialized
    /// with the three pins. The channel is also initialized.
    led_controller(const char * task_name, hwlib::pin_out & r, hwlib::pin_out & g, hwlib::pin_out & b):
        task(4, task_name),
        led_color_channel(this, "led_color_channel"),
        led(r, g, b)
    {}
    
    ///Write to the led color channel
    //
    ///This function makes sure other objects can write to the channel.
    ///It dus not return anything and needs a led color behaviour
    /// structure as parameter.
    void write(led_color_behaviour color){
        led_color_channel.write(color);
    }
};

#endif // RGB_LED_CLASSES_HPP
