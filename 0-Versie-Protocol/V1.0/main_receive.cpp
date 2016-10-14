#include "hwlib.hpp"
#include "receive_ir_message.hpp"

int main(void){
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
    hwlib::cout << "Starting program .. \n";
    auto receiver = receive_ir_message(r_pin);
    receiver.get_decoded_byte();
}