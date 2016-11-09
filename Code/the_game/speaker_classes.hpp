/// @file
#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef SPEAKER_CLASSES_HPP
#define SPEAKER_CLASSES_HPP

///Data structure for sound_storage
//
///structure for storage of frequency, duration,
///amount( of tones), amount_wait(time between tones in ms)
struct sound_lookup{
    int frequency;
    int duration;
    int amount;
    int amount_wait;
};

///speaker class
//
///Simple player class for playing frequency's on a speaker connected
/// to a GPIO pin. Created by Wouter van Ooijen.(modified)
class speaker{
private:   
   hwlib::pin_out & speaker_pin;
public: 
    ///Default constructor
    //
    ///This constructor receives the pin the speaker is connected to
    /// and initializes the variable.
   speaker( hwlib::pin_out & speaker_pin ):
      speaker_pin( speaker_pin )
   {}
    ///Play function
    //
    ///Play sound by given parameters frequency, duration, amount, amount_wait.
    ///Frequency is used in a formule to math half period(time between speaker on and speaker off).
    ///Duration is the amount of systemtime the sound is ale to play.
    ///Amount is how many times the sound will play.
    ///Amount_wait is the time between everytime the sound is playing.
    ///This function dus not return anything.
    void play( const uint_fast16_t frequency, const uint_fast16_t duration,const uint_fast16_t amount,const uint_fast16_t amount_wait )  {
        if( frequency == 0 ){
            hwlib::wait_us( duration );
        }
        else{
            auto half_period = 1'000'000 / ( 2 * frequency ); 
            for(uint_fast16_t i= 0; i < amount; i++){ 
                auto end = hwlib::now_us() + duration;
                
                while( end > hwlib::now_us() ){
                    speaker_pin.set( 1 );
                    hwlib::wait_us( half_period );
                    speaker_pin.set( 0 );
                    hwlib::wait_us( half_period );
                }
                hwlib::wait_ms(amount_wait);
            }
        }
    }
};

///Speaker controller class
//
///This class is used for controlling the speaker and 
/// converting the sound_lookup struct received from the sound pool
/// to useable variables. Speaker_controller uses the concurrency mechanism 
/// pool with mutex for receiving which sound is needed to be played.
/// It also uses a flag to know when a new sond is ready to be read from
/// the pool and played.
class speaker_controller :public rtos::task<> {
private:
    speaker sound_device;
    rtos::flag play_sound_flag;
    rtos::pool<sound_lookup> sound_pool;
    rtos::mutex & sound_mutex;
    
    ///Main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. The function waits for the sound changed
    /// flag to be set and then reads the sound information from
    /// the pool.The information is used toe play a sound.
    void main(){
        while(1){
            wait(play_sound_flag);
            sound_mutex.wait();
            auto sound_file = sound_pool.read();
            sound_mutex.signal();
            play_sound(sound_file);
        }
    }
public:
    /// shoot sound sound_lookup 
    sound_lookup shoot{4000,40000, 3, 50};
    /// hit sound sound_lookup
    sound_lookup hit{500,50000, 3,200};
    
    ///Default constructor
    //
    ///This constructor receives a task name, speaker pin and sound mutex as
    ///parameters. It initializes these together with the flag and pool.
    speaker_controller(const char * task_name, hwlib::pin_out & speaker_pin, rtos::mutex & sound_mutex):
        task(5, task_name),
        sound_device(speaker_pin),
        play_sound_flag(this,"play_sound_flag"),
        sound_pool("speaker_pool"),
        sound_mutex(sound_mutex)
    {}
    
    ///play sound
    //
    ///This function plays the sound from a received sound lookup.
    ///It calls the play function from the sound device. The
    /// function returns nothing.
    void play_sound(const sound_lookup & play){
    sound_device.play(play.frequency,play.duration,play.amount,play.amount_wait);
        //hwlib::cout<< "\n"<< play.frequency<<play.duration<<play.amount<<play.amount_wait;
    }
    
    ///write to sound pool
    //
    ///This function is used to make sure other classes can write
    /// to the sound pool. It returns nothing and requires a 
    ///sound lookup type as parameter.
    void write(const sound_lookup sound_struct){
        sound_pool.write(sound_struct);
    }
    
    ///Set the play sound flag
    //
    ///This function is used to make other objects able to set the
    /// play sound flag. It returns nothing and requires no parameters.
    void enable_flag(){
            play_sound_flag.set();
        }
};

#endif // SPEAKER_CLASSES_HPP
