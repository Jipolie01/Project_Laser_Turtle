#include <stdio.h>

int main(int argc, char **argv)
{
    //Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    
    
    run_game_controller(lcd);
	rtos::run();
}
