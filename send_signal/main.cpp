#include "hwlib.hpp"

int main(int argc, char **argv){
       // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    auto ir = hwlib::target::d2_36kHz();
    auto sw = hwlib::target::pin_in(hwlib::target::pins::d7);
   
    char vb = 0b10001001;
    
    while(1){
        if(!sw.get()){
            hwlib::cout << "opnieuw\n";
            bool a;
            for(int j = 0; j < 2; j++){
                for (int i = 0; i < 8; i++){
                    a = (vb & (1 << (7 - i)));
                    if(a == 1){
                        ir.set(1);
                        hwlib::wait_us(1600);
                        ir.set(0);
                        hwlib::wait_us(800);
                    }else{
                        ir.set(1);
                        hwlib::wait_us(800);
                        ir.set(0);
                        hwlib::wait_us(1600);
                    }
                }
                hwlib::wait_ms(3);
            }
        }
        hwlib::wait_ms(200);
    }
	return 0;
}