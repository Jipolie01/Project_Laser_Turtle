#include "hwlib.hpp"
#include "rtos.hpp"

class listener;

class button{
protected:
    listener* the_listener = 0;
    bool down_seen = 0;
    hwlib::pin_in & button_pin;
public:
    button( hwlib::pin_in &button_pin );
    void add_listener( listener *lst );
    void update_button_state( void );
};

class listener{
public:
    virtual void button_pressed() = 0;
};

class button_controller: public listener,public rtos::task<>{
private:
    hwlib::pin_in & button_pin;
    rtos::clock check_clock;
    button b;
    rtos::flag & button_pressed_flag;
    
    void main(void){
        b.add_listener(this);
        while(1){
            wait(check_clock);
            b.update_button_state();
        }
    }
    
    
public:
    button_controller(hwlib::pin_in & button_pin, rtos::flag & button_pressed_flag);
    void button_pressed() override;
    
    
};