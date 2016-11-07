#include "hwlib.hpp"
#include "rtos.hpp"

#ifndef SPEAKER_CLASSES_HPP
#define SPEAKER_CLASSES_HPP

///Simple player
//
///Simple player for playing frequency's on a speaker connected
/// to a GPIO pin. Created by Wouter van Ooijen.

struct sound_lookup{
// int frequenty and duration
    int frequency;
    int duration;
    int count;
    int count_wait;
};


class speaker{
private:   
   hwlib::pin_out & speaker_pin;
public: 
   speaker( hwlib::pin_out & speaker_pin ):
      speaker_pin( speaker_pin )
   {}
   
   void play_frequency( const uint_fast16_t frequency, const uint_fast16_t duration,const uint_fast16_t count,const uint_fast16_t count_wait )  {
        if( frequency == 0 ){
         hwlib::wait_us( duration );
        }
        else{
         auto half_period = 1'000'000 / ( 2 * frequency ); 
        for(uint_fast16_t i= 0; i < count; i++){ 
        auto end = hwlib::now_us() + duration;
         
             while( end > hwlib::now_us() ){
                 
                speaker_pin.set( 1 );
                hwlib::wait_us( half_period );
                speaker_pin.set( 0 );
                hwlib::wait_us( half_period );
         
          } 
            hwlib::wait_ms(count_wait);
        }
      }     
   }
};



class speaker_controller :public rtos::task<> {
private:
    speaker sound_device;
    rtos::flag play_sound_flag;
    rtos::pool<sound_lookup> sound_pool;
    rtos::mutex sound_mutex;
   
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

sound_lookup shoot{4000,40000, 3, 50};

sound_lookup hit{500,50000, 3,200};

    speaker_controller(const char * task_name, hwlib::pin_out & speaker_pin,rtos::mutex & sound_mutex):
        task(task_name),
        sound_device(speaker_pin),
        play_sound_flag(this,"play_sound_flag"),
        sound_pool("speaker_pool"),
        sound_mutex(sound_mutex)
        
    {}
    
    
    
        void print(const sound_lookup & sound_code){
       hwlib::cout<< sound_code.frequency;
       
   }   
void play_sound(const sound_lookup & play){
sound_device.play_frequency(play.frequency,play.duration,play.count,play.count_wait);
    hwlib::cout<< "\n"<< play.frequency<<play.duration<<play.count<<play.count_wait;
}
void write(const sound_lookup sound_struct){
    sound_pool.write(sound_struct);
}

void enable_flag(){
        play_sound_flag.set();
    }

};

#endif // SPEAKER_CLASSES_HPP
