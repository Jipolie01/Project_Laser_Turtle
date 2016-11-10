#include "listenerpattern.hpp"

button::button(hwlib::pin_in & button_pin):
    button_pin(button_pin)
{}

void button::add_listener(listener *lst)
{
    the_listener = lst;
}

void button::update_button_state( void )
{
    if((!button_pin.get()) && (!down_seen)){
        the_listener->button_pressed();
        down_seen = 1;
    }else if((down_seen) &&(button_pin.get())){
        hwlib::cout << "button released \n";
        down_seen = 0;
    }
}

button_controller::button_controller(hwlib::pin_in & button_pin):
    task("button_task"),
    button_pin(button_pin),
    check_clock(this,200 * rtos::ms,"check-clock"),
    b(button_pin)
{}

void button_controller::button_pressed()
{
        hwlib::cout << "button pressed \n";
        //set flag
}
