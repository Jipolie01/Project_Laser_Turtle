#ifndef APPLICATION_LOGIC_CLASSES_HPP
#define APPLICATION_LOGIC_CLASSES_HPP

class ir_message_logic{
public:
    ir_message_logic()
    {}
    
    char16_t encode(uint_fast8_t player_id, uint_fast8_t weapon_id){
        char16_t compiled_message = 1;
        compiled_message = compiled_message << 5;
        compiled_message = (compiled_message | player_id);
        compiled_message = compiled_message << 5;
        compiled_message = (compiled_message | weapon_id);
        compiled_message = compiled_message << 5;
        compiled_message = (compiled_message | (player_id ^ weapon_id));
        return compiled_message;
    }
};

#endif // APPLICATION_LOGIC_CLASSES_HPP
