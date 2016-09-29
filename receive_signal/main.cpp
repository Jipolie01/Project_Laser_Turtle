#include "hwlib.hpp"
/*
int main( void ){	
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   
   namespace target = hwlib::target;
   
   auto led0 = target::pin_out( target::pins::d7 );
   auto led1 = target::pin_out( target::pins::d6 );
   auto led2 = target::pin_out( target::pins::d5 );
   auto led3 = target::pin_out( target::pins::d4 );
   
   auto leds = hwlib::port_out_from_pins( led0, led1, led2, led3 );
   hwlib::kitt( leds );
}
 */
bool receiver(hwlib::pin_in & receive_pin){
    hwlib::cout << "plek 1";
    while(receive_pin.get() == 1);
    hwlib::cout << "plek 2";
    auto time1 = hwlib::now_us();
    hwlib::cout << "plek 3";
    while(receive_pin.get() == 0);
    hwlib::cout << "plek 4";
    
    auto time2 = hwlib::now_us();
    auto result_time = time2-time1;

    if(result_time == 1600){
        return true;
    }
    else{
        return false;
    }
}

int main(void){
    hwlib::wait_ms(1000);
    hwlib::cout << "start";
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    auto r_pin = hwlib::target::pin_in(hwlib::target::pins::d8);
    auto gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    auto vcc = hwlib::target::pin_out(hwlib::target::pins::d10);

    gnd.set(0);
    vcc.set(1);

    while(1){
        //hwlib::cout << "check";
        hwlib::cout << receiver(r_pin);
        hwlib::cout << '\n';
        //hwlib::wait_ms(1);
    }
    return 0;
}
