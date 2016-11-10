#include "hwlib.hpp"
#include "run_game.hpp"

#ifndef IR_RECEIVER_CLASSES_HPP
#define IR_RECEIVER_CLASSES_HPP

/*
/// ir_receiver
//
/// This class contains all methods to receive bits. 
/// Note: this is the only class that doesn't has any rtos-functionality, this is mainly 
/// because of the lack of time there was left for this project. When this class is transformed
/// into a task it doesn't work anymore, this was also a reason to leave it without rtos.
class ir_receiver{
private:
    hwlib::pin_in & r_pin;
    hwlib::pin_out & gnd;
    hwlib::pin_out  & vcc;
public:
    ///Default constructor
    //
    /// Parameters: one input pin, two output pins for the ground and vcc
    /// This constructor initializez the parameter pins given, and sets the 
    /// ground on 0 and the vcc on 1.
    ir_receiver(hwlib::pin_in & r_pin, hwlib::pin_out & gnd, hwlib::pin_out &vcc):
        r_pin( r_pin),
        gnd( gnd),
        vcc(vcc)
    {
        gnd.set(0);
        vcc.set(1);
    }
    
};*/

/// ir_receiver_controller
//
/// This class adds extra functionality to ir_receiver
class ir_receiver_controller : public rtos::task<>{
private:
    rtos::timer pool_receiver_timer;
    hwlib::pin_in & r_pin;
    hwlib::pin_out & gnd;
    hwlib::pin_out  & vcc;
    
    
    run_game_controller & run_game;
    
    void main(void){
        while(1){
            pool_receiver_timer.set(400*rtos::us);
            wait(pool_receiver_timer);
            if(get_start_bit() != -1){
                run_game.write(get_message());
                sleep(100*rtos::ms);
            }
        }
    }
public:
    /// ir_receiver_controller
    //
    /// Parameters: one input pin, two output pins to define the ir_receiver
    /// object
    ir_receiver_controller(hwlib::pin_in & r_pin, hwlib::pin_out & gnd, hwlib::pin_out & vcc, run_game_controller & run_game):
    task(0,"ir_receiver_controller"),
    pool_receiver_timer(this, "pool_receiver_timer"),
    r_pin(r_pin),
    gnd(gnd),
    vcc(vcc),
    run_game(run_game)
    {
        gnd.set(0);
        vcc.set(1);
    }
    
    /// get_start_bit
    //
    /// This method is used to receive the first bit of a message
    /// Return value: This function returns a 1 or 0 depending on 
    /// the value of the bit send. If there is no bit send it returns
    /// -1.
    int get_start_bit()
    {
         if(!r_pin.get()){
            hwlib::wait_us(1100);
            if(!r_pin.get()){
                hwlib::wait_us(700);
                return 1;
            }else{
                hwlib::wait_us(700);
                return 0;
            }
        }
        return -1;
    }
    
    int get_bit(){
        int begin = hwlib::now_us();
        while(r_pin.get()){
            sleep(100 * rtos::us);
            if(begin-hwlib::now_us() >= 4000){
                return -1;
            }
        }
            sleep(1100*rtos::us);
            if(!r_pin.get()){
                sleep(700*rtos::us);
                return 1;
            }else{
                sleep(700*rtos::us);
                return 0;
            }
        return -1;
    }
    
    
    
    /// get_bit
    //
    /// This method tries to get a bit. When this doesn't happen within 4 ms 
    /// the method will return -1, otherwise it will return the received bit
    /*int get_bit(){
        int bit;
        auto begin_time = hwlib::now_us();
        while((hwlib::now_us() - begin_time) < 4000){
            bit = get_start_bit();
            if(bit != -1){
                return bit;
            }
        }
        return -1;
    }*/
    
    /// get_message
    //
    /// This method is used to receive the whole message. This method can only be called
    /// after the start_bit is received.
    /// Return value: char16_t number that represent the message 
    char16_t get_message(){
        char16_t bitstream = 0;
        bitstream = bitstream << 1;
        for(int i = 0; i < 15; i++){
            auto bit = get_bit();
            if((bit == 1) || (bit == 0)){
            bitstream = (bitstream | bit);
            }else if(bit == -1){
                return -1;
            }
            if(i < 15){
                bitstream = bitstream << 1;
            }          
        }
        bitstream = bitstream | (1 << 15);
        hwlib::cout << bitstream << '\n';
        return bitstream;
    }
    /*
    int get_start_bit(){
        return receiver.get_start_bit();
    }*/
    
    /// recieve_full_message
    //
    /// This method receives the full message from start_bit to printing the message on the screen
    /// Return value: 1 when succesful, 0 when unsuccesfull
    /*int receive_full_message(){
        auto byte = receiver.get_message();
        if(byte == -1){
            return -1;
        }else{
            hwlib::cout << byte << '\n';
            return byte;
        }
    }*/

};

#endif // IR_RECEIVER_CLASSES_HPP