#include "hwlib.hpp"

int get_bit(auto &p1){
    
    for(;;){
        if ( p1.get() == 0){
            hwlib::wait_us(1000);
            if (p1.get() == 0){
                hwlib::wait_us(700);
                hwlib::cout <<"1\n";
            }
            else if (p1.get() == 1){
                hwlib::cout <<"0\n";
            }
        }
}}


int main(int argc, char **argv)
{
	//Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    //Defining pins
    auto r_pin = hwlib::target::pin_in(hwlib::target::pins::d8);
    auto gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto vcc = hwlib::target::pin_out(hwlib::target::pins::d10);
    
    //Setting ground and vcc of the receiver
    gnd.set(0);
    vcc.set(1);


    //Running program
    while(1){
        get_bit(r_pin);
    }
}
