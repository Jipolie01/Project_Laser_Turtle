#include "hwlib.hpp"

#ifndef ENTITY_CLASSES_HPP
#define ENTITY_CLASSES_HPP

class my_player_information {
private:
    char16_t compiled_bits;
public:
    my_player_information():
        compiled_bits(0)
    {}
    
    char16_t get_compiled_bits(){
        return compiled_bits;
    }
    
    void set_compiled_bits(char16_t value){
        compiled_bits = value;
    }

};


class game_information_data {
private:
    int game_time_remaining;
    bool game_has_started;
public:
    game_information_data():
        game_time_remaining(0),
        game_has_started(0)
    {}
    
    int get_game_time(){
        return game_time_remaining;
    }
    
    bool get_game_has_start(){
        return game_has_started;
    }
    
    void set_game_time(int time){
        game_time_remaining = time;
    }
    
    void set_game_has_start(bool yes_no){
        game_has_started = yes_no;
    }

};

class hit{
private:
    byte player_id;
    byte weapon_id;
public:
    hit(byte player_id, byte weapon_id):
        player_id(player_id),
        weapon_id(weapon_id)
    {}
    
    byte get_player_id(){
        return player_id;
    }
    
    byte get_weapon_id(){
        return weapon_id;
    }
};

#endif // ENTITY_CLASSES_HPP
