#include "register_game_parameters.hpp"
//#include "display_classes.hpp"

int main(){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    // wait for the PC console to start
    hwlib::wait_ms( 500 );
   
    //define pins
    hwlib::target::pin_oc out0 = hwlib::target::pin_oc( hwlib::target::pins::a0 );
    hwlib::target::pin_oc out1 = hwlib::target::pin_oc( hwlib::target::pins::a1 );
    hwlib::target::pin_oc out2 = hwlib::target::pin_oc( hwlib::target::pins::a2 );
    hwlib::target::pin_oc out3 = hwlib::target::pin_oc( hwlib::target::pins::a3 );
    hwlib::port_oc_from_pins out_port = hwlib::port_oc_from_pins( out0, out1, out2, out3 );
    hwlib::target::pin_in in0 = hwlib::target::pin_in( hwlib::target::pins::a4 );
    hwlib::target::pin_in in1 = hwlib::target::pin_in( hwlib::target::pins::a5 );
    hwlib::target::pin_in in2 = hwlib::target::pin_in( hwlib::target::pins::a6 );
    hwlib::target::pin_in in3 = hwlib::target::pin_in( hwlib::target::pins::a7 );
    hwlib::port_in_from_pins in_port = hwlib::port_in_from_pins( in0, in1, in2, in3 );
    
    //make the necessary objects
    Keypad keypad(out_port, in_port);
    ir_message_logic compile_message;
    my_player_information player_information;
    register_game_parameters_controller get_game_parameters(compile_message, player_information, keypad);

    //set init done and key to 0
    bool reg_done = 0;
    char key = 0;
    
    //loop and get new character input till register game parameters is done
    while(1){
        key = keypad.check_for_input();
        reg_done = get_game_parameters.setup(key);
        hwlib::wait_ms(200);
        if(reg_done == 1) break;
    }
    
    hwlib::cout << "done \n";
    char16_t bits = player_information.get_compiled_bits();
    hwlib::cout << bits;
}