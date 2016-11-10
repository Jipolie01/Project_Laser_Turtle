/// @file
#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef LISTENERPATTERN_HPP
#define LISTENERPATTERN_HPP

///Forward declarations
class listener;
class run_game_controller;

/// button
//
/// This class is used for the functionality of the button 
class button{
protected:
    listener* the_listener = 0;
    bool down_seen = 0;
    hwlib::pin_in & button_pin;
public:
    /// constructor
    //
    /// Parameters: input pin
    /// This constructor is used to define the pin where it should be read
    button( hwlib::pin_in &button_pin );
    /// add_listener
    //
    /// Parameter: listener pointer refering to class that is listening to the button
    /// This method is used to add a class that is listening to the button 
    void add_listener( listener *lst );
    /// update_button_state
    //
    /// This method is used to update the state for the button. This method is used 
    /// to call the button_pressed function from the pointer given to add_listener
    void update_button_state( void );
};

/// Listener
//
/// This class is used to listen to the button
class listener{
public:
    /// button_pressed
    //
    /// this method is abstract and is used by button controller
    virtual void button_pressed() = 0;
};

/// button_controller
//
/// This class is also a task and can be used to listen to the button.
class button_controller: public listener,public rtos::task<>{
private:
    hwlib::pin_in & button_pin;
    rtos::clock update_button_state_clock;
    button b;
    run_game_controller * run_game;
    
    /// main
    //
    /// this method is used a main for the function. This method waits for a clock to set.
    /// When the clock is activated, the button state is updated  
    void main(void){
        b.add_listener(this);
        while(1){
            wait(update_button_state_clock);
            b.update_button_state();
        }
    }
    
    
public:
    /// default constructor
    //
    /// this constructor is used to intialize the task and the pin used for the button
    button_controller(hwlib::pin_in & button_pin, run_game_controller * run_game);
    /// button_pressed
    //
    /// this method gives functionality to the button. This method is called when the button
    /// is pressed. When the button is pressed the button sets the flag in run_game
    void button_pressed() override;
    
    
};

#endif //LISTENERPATTERN_HPP