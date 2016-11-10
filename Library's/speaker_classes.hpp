#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef SPEAKER_CLASSES_HPP
#define SPEAKER_CLASSES_HPP

///Simple player
//
///Simple player for playing frequency's on a speaker connected
/// to a GPIO pin. Created by Wouter van Ooijen.(modified)

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
///class for output for playing sound used bij a 2 pin speaker

class speaker{
private:   
///pin out reference speaker 
   hwlib::pin_out & speaker_pin;
public: 
///speaker constructor
   speaker( hwlib::pin_out & speaker_pin ):
      speaker_pin( speaker_pin )
   {}
/// play function
//
///play sound by given parameters frequency, duration, amount, amount_wait
///frequency is used in a formule to math half period(time between speaker on and speaker off)
///duration is the amount of systemtime the sound is ale to play
///amount is how many times the sound will play
///amount_wait is the time between everytime the sound is playing
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

///speaker_controller
//
///controller that inherence rtos::task 
///used for controlling the speaker and 
///convert sound_lookup struct to useable variables
///speaker_controller uses concurrency mechanism 
///pool with mutex for sending which sound is needed to be played
///flag for showing if can play

class speaker_controller :public rtos::task<> {
private:
    speaker sound_device;
    rtos::flag play_sound_flag;
    rtos::pool<sound_lookup> sound_pool;
    rtos::mutex sound_mutex;
/// function main

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
/// speaker_controller constructor
speaker_controller(const char * task_name, hwlib::pin_out & speaker_pin,rtos::mutex & sound_mutex):
        task(task_name),
        sound_device(speaker_pin),
        play_sound_flag(this,"play_sound_flag"),
        sound_pool("speaker_pool"),
        sound_mutex(sound_mutex)
        
    {}
    
    
/// function play_sound
//
/// get the values out sound_lookup struct and play them.
void play_sound(const sound_lookup & play){
sound_device.play(play.frequency,play.duration,play.amount,play.amount_wait);
    //hwlib::cout<< "\n"<< play.frequency<<play.duration<<play.amount<<play.amount_wait;
}
///function write 
//
/// write sound_pool
void write(const sound_lookup sound_struct){
    sound_pool.write(sound_struct);
}
/// function enable_flag
//
/// set the play_sound_flag
void enable_flag(){
        play_sound_flag.set();
    }

};

#endif // SPEAKER_CLASSES_HPP
