#include "hwlib.hpp"
#include "send_ir_message.hpp"

int main(){
    //Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    //Defining pins
    auto ir = hwlib::target::d2_36kHz();
    auto button = hwlib::target::pin_in(hwlib::target::pins::d7);
    //Defining class objects    
    auto receiver = send_ir_message(ir,2,2);

    //Running program
    for(;;){
        hwlib::wait_ms( 1 ); 
        if (! button.get() == 1){
            hwlib::cout << "Sending byte \n";
            receiver.send_byte();
            hwlib::wait_ms(1000);
        }     
        hwlib::wait_ms( 1 );
        ir.set( 0 );
   }
    
    
    return 0;
}