#include "hwlib.hpp"

void receiver(hwlib::pin_in & receive_pin){
   // hwlib::cout << "plek 1";
   if(receive_pin.get() == 1){
       for(int i = 0; i < 8; i++){
               while(receive_pin.get() == 1);
           // hwlib::cout << "plek 2";
                auto time1 = hwlib::now_us();
           //hwlib::cout << "plek 3";
                while(receive_pin.get() == 0);
           // hwlib::cout << "plek 4";
                auto time2 = hwlib::now_us();
                auto result_time = time2-time1;
               
                if result_time >= 4000000 break;
                if(result_time >= 800 && result_time <= 900) hwlib::cout << '1';
                if(result_time >= 1600 && result_time <= 1700) hwlib::cout << '0';
        }
        hwlib::cout << '\n';
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
        receiver(r_pin);
        //hwlib::wait_ms(1);
    }
    return 0;
}
