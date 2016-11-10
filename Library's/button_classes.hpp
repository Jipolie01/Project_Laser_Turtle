#include "hwlib.hpp"

#ifndef BUTTON_CLASSES_HPP
#define BUTTON_CLASSES_HPP

///button listener
//
///This abstract class is used listen to all sorts of different buttons.
///Using the listener you can do this in the exact same way for
/// every button regardless of what the button classes are called.
class button_listener {
public:
    ///Default constructor
    //
    ///This constructor requires a pin to wich the button is attached.
    button_listener()
    {}
    
    ///Default destructor
    //
    ///Used to remove objects and variabeles created by this class
    /// to free up memory.
    ~button_listener();
    
    ///Check input
    //
    ///This function checks for the input. It needs to be defined
    /// in other classes that inherit this one
    virtual bool button_down() = 0;
};

///button class
//
///This class defines how our button is to be read. It inherits
/// button listener to generalize the reading of the button.
class button : public button_listener{
protected:
    hwlib::pin_in & button_pin;
public:
    ///Default constructor
    //
    ///The constructor receives a pin on creation to wich the
    /// button is attached and creates a button_listener object
    /// to be used for the generalization.
    button(hwlib::pin_in & button_pin):
        button_pin(button_pin)
    {}
    
    ///check button
    //
    ///This function defines how our button should work. It overrides
    /// the function in the button_listener class to make generalization
    /// work.
    bool button_down() override {
        return !(button_pin.get());
    }
};


#endif // BUTTON_HANDLER_HPP
