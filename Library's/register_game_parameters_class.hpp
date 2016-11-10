#include "entity_classes.hpp"
#include "application_logic_classes.hpp"
#include "keypad_class.hpp"

#ifndef REGISTER_GAME_PARAMETERS_CLASS_HPP
#define REGISTER_GAME_PARAMETERS_CLASS_HPP

///register_game_parameters_controller
//this class registers a player and weapon id and it encodes a message
class register_game_parameters_controller{
private:
    ir_message_logic & compile;
    my_player_information & information;
    Keypad & keypad;
    char key;
    bool a_is_pressed;
    bool b_is_pressed;
    bool weapon_id_pressed;
    bool player_id_pressed;
    char player_id_char;
    char weapon_id_char;

public:

    register_game_parameters_controller(ir_message_logic & compile, my_player_information & information, Keypad & keypad):
        compile(compile),
        information(information),
        keypad(keypad),
        a_is_pressed(0),
        b_is_pressed(0),
        weapon_id_pressed(0),
        player_id_pressed(0),
        player_id_char(0),
        weapon_id_char(0)
    {}

    bool setup(char key){
        if((key == 'A') && a_is_pressed == 0){
            a_is_pressed = 1;
            return 0;
        }
        if((((key >= '0') && (key <= '9')) && a_is_pressed == 1) && player_id_pressed == 0){
            player_id_char = key;
            player_id_pressed = 1;
            return 0;
        }
        if(((key == 'B') && player_id_pressed == 1) && b_is_pressed == 0){
            b_is_pressed = 1;
            return 0;
        }
        if((((key >= '0') && (key <= '9')) && b_is_pressed == 1) && weapon_id_pressed == 0){
            weapon_id_char = key;
            weapon_id_pressed = 1;
            uint_fast8_t player_id = player_id_char - '0';
            uint_fast8_t weapon_id = weapon_id_char - '0';
            char16_t compiled = compile.encode(player_id, weapon_id);
            information.set_compiled_bits(compiled);
            return 1;
        }
        return 0;
    }
};

#endif //REGISTER_GAME_PARAMETERS_CLASS_HPP