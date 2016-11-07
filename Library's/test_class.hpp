#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef TEST_CLASS_HPP
#define TEST_CLASS_HPP

template < typename T>
class test_class : public rtos::task<>{
private:
    T & controller;
    
    void main(void){
        hwlib::cout << "Starting program .. \n";
        while(1){
            
        }
    }
public:
    test_class(T & controller):
        task("test_class"),
        controller(controller)
    {}
};

#endif // TEST_CLASS_HPP
