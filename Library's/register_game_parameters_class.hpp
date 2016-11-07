#include "keypad.hpp"
//#include "display_classes.hpp"
#include "entity_classes.hpp"
#include "application_logic_classes.hpp"

class register_game_parameters_controller{
private:
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
  //  rtos::mutex & lcd_mutex;
  //  rtos::pool<> lcd_pool;
  //  rtos_flag lcd_information_changed_flag;
    ir_message_logic & compile;
    my_player_information & information;
    Keypad keypad;

public:
register_game_parameters_controller(ir_message_logic & compile, my_player_information & information): compile(compile), information(information), keypad(out_port, in_port) {}

void send(char & c){
 //     lcd_mutex.wait();
 //     lcd_pool.write(c);
  //    lcd_mutex.signal();
  //    lcd_information_changed_flag.set();
      hwlib::cout << " [" << c << "]\n";
}
        
void setup(){
    char key;
    while(key != 'A'){
        key = keypad.check_for_input();
        hwlib::wait_ms(200);
    }
    send(key);
    while((key < '1') || (key > '9')){
        key = keypad.check_for_input();
        hwlib::wait_ms(200);
    }
    char player_id_char = key;
    send(key);
    while(key != 'B'){
        key = keypad.check_for_input();
        hwlib::wait_ms(200);
    }
    send(key);
    while((key < '1') || (key > '9')){
        key = keypad.check_for_input();
        hwlib::wait_ms(200);
    }
    char weapon_id_char = key;
    send(key);
    uint_fast8_t player_id = player_id_char - '0';
    uint_fast8_t weapon_id = weapon_id_char - '0';
    
    char16_t compiled = compile.encode(player_id, weapon_id);
    information.set_compiled_bits(compiled);
}
};