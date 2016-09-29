#include "hwlib.hpp"

int main(int argc, char **argv){
       // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;

   auto ir = hwlib::target::d2_36kHz();
   
    char vb = 0b10001001;
    while(1){
        ir.set(0);
        hwlib::wait_ms(1000);
        hwlib::cout << "opnieuw\n";
        bool a;
        for (int i = 0; i < 8; i++){
            a = (vb& (1 << (7 - i)));
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
    }
	return 0;
}