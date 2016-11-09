/// @file
#include "hwlib.hpp"

#ifndef ENTITY_CLASSES_HPP
#define ENTITY_CLASSES_HPP

///Save the player information
//
///This class contains the health of a player and the
/// compiled bits that need to be send when the player
/// shoots.
class my_player_information {
private:
    int health;
    char16_t compiled_bits;
public:
    ///Default constructor
    //
    ///Zero initializes the compiled bits because those
    /// will be compiled later. It also initializes the
    /// health of the player. It has no parameters.
    my_player_information():
        health(100),
        compiled_bits(0)
    {}
    
    ///get compiled bits
    //
    ///Gives other objects access to the private variable
    ///compiled bits. Returns the compiled bits. It
    /// dus not require any parameters.
    char16_t get_compiled_bits(){
        return compiled_bits;
    }
    
    ///get player health
    //
    ///Gives other objects access to the private variable
    ///health. Returns the health. It
    /// dus not require any parameters.
    int get_health(){
        return health;
    }
    
    ///Set compiled bits
    //
    ///This function is used to set the compiled bits.
    ///It requires a char16_t value and dus not return
    /// anything.
    void set_compiled_bits(char16_t value){
        compiled_bits = value;
    }
    
    ///Set player health
    //
    ///This function is used to set the player health.
    ///It requires a integer value and dus not return
    /// anything.
    void set_health(int value){
        health = value;
    }

};

///Store game information
//
///This entity class is used to store all the game
/// parameters. This includes the cooldown time
/// , the game time and a check for other objects
/// to see if the game has started.
class game_information_data {
private:
    int cooldown_time;
    int game_time_remaining;
    bool game_has_started;
public:
    ///Default constructor
    //
    ///This constructor zero initializes all data in
    /// this entity class. It dus not require any
    /// parameters.
    game_information_data():
        cooldown_time(0),
        game_time_remaining(0),
        game_has_started(0)
    {}
    
    ///get game time
    //
    ///Gives other objects access to the private variable
    ///game_time. Returns the game time. It
    /// dus not require any parameters.
    int get_game_time(){
        return game_time_remaining;
    }
    
    ///get game has started
    //
    ///Gives other objects access to the private variable
    ///game started. It dus not require any parameters.
    bool get_game_has_start(){
        return game_has_started;
    }
    
    ///get cooldown time
    //
    ///Gives other objects access to the private variable
    ///cooldown time. It dus not require any parameters.
    int get_cooldown_time(){
        return cooldown_time;
    }
    
    ///Set game time
    //
    ///This function is used to set the game time.
    ///It requires a integer value and dus not return
    /// anything.
    void set_game_time(int time){
        game_time_remaining = time;
    }
    
    ///Set game has started
    //
    ///This function is used to set the game has started variable.
    ///It requires a boolean value and dus not return
    /// anything.
    void set_game_has_start(bool yes_no){
        game_has_started = yes_no;
    }
    
    ///Set cooldown time
    //
    ///This function is used to set the cooldown time.
    ///It requires a integer value and dus not return
    /// anything.
    void set_cooldown_time(int time){
        cooldown_time = time;
    }
};


///Store a hit
//
///Used to store hits made by other players on you.
///This data is beeing stored to be used to display
/// on a computer for competitive matches. The
/// received player id and weapon id are beeing stored.
class hit{
private:
    byte player_id;
    byte weapon_id;
public:
    ///Default constructor
    //
    ///The constructor initializes the player id and
    /// weapon id. These are given to the constructor
    /// as parameters.
    hit(byte player_id, byte weapon_id):
        player_id(player_id),
        weapon_id(weapon_id)
    {}
    
    ///get player id
    //
    ///Gives other objects access to the private variable
    ///player_id. It dus not require any parameters.
    byte get_player_id(){
        return player_id;
    }
    
    ///get weapon id
    //
    ///Gives other objects access to the private variable
    ///weapon_id. It dus not require any parameters.
    byte get_weapon_id(){
        return weapon_id;
    }
};

#endif // ENTITY_CLASSES_HPP
