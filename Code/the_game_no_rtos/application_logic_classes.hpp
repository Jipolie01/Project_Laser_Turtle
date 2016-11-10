/// @file

#ifndef APPLICATION_LOGIC_CLASSES_HPP
#define APPLICATION_LOGIC_CLASSES_HPP

///Ir message logic class
//
///This class contains all the calculations needed for the ir messages.
class ir_message_logic{
public:
    ///Default constructor
    //
    ///The constructor dus not need any parameters nor dus it initialize
    /// anything. It is just there to create an object from this class.
    ir_message_logic()
    {}
    
    ///Encode ir messages
    //
    ///This function is used to compile a char16_t code that is
    /// going to be send by the ir led. The function requires two
    /// eight bit integer values and returns the char16_t code it
    /// compiled. The code consists of a start bit. then five bits
    /// for the player id. After that 5 bits for the weapon id or for
    /// commands. Lastly the last five bits are for the exor check bits.
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
    
    ///Decode the bitstream
    //
    ///This function decodes the bitstream in the way specified in the protocol part of the casus.
    ///It also checks the bitstream with an exor to make sure it is authentic.
    /// The function needs the bitstream as a parameter and returns zero or one depending
    /// on the result of the exor. One being exor correct and zero exor incorrect.
    bool decode(char16_t receiving_bits, byte & received_player_id, byte & received_weapon_id){
        byte exor = (receiving_bits & 31);
        byte identifier_weapon = ((receiving_bits >> 5) & 31);
        byte identifier_player = ((receiving_bits >> 10) & 31);
        
        if((identifier_player ^ identifier_weapon) == exor){
            received_player_id = identifier_player;
            received_weapon_id = identifier_weapon;
            return 1;
        }
        else{
            return 0;
        }
    }
};

#endif // APPLICATION_LOGIC_CLASSES_HPP
