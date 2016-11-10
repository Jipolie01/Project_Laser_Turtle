/// @file
#include "hwlib.hpp"
#include "run_game.hpp"

#ifndef IR_RECEIVER_CLASSES_HPP
#define IR_RECEIVER_CLASSES_HPP

///ir_receiver_controller
//
///This class is the ir receiver class. It is a boundary object
/// and controller object in one because we needed rtos functionality.
///The class uses a timer to check the ir receiver periodically.
class ir_receiver_controller : public rtos::task<>{
private:
    rtos::timer pool_receiver_timer;
    hwlib::pin_in & r_pin;
    hwlib::pin_out & gnd;
    hwlib::pin_out  & vcc;
    
    run_game_controller & run_game;
    
    ///main function
    //
    ///Function automatically called by the rtos. All actions
    /// are done in this function. It takes no arguments and
    /// returns nothing. It starts by initializing the game
    /// parameters. This function first sets the timer to 400
    /// mirco seconds. When that time expires the ir receiver
    /// is used to check if a start bit is beeing send.
    ///If a start bit is beeing send the receiver starts
    /// looking for the rest of the bits. After it receives
    /// the full message it sleeps for 5000 miliseconds.
    /// This delay is for the fact that you can not be shot
    /// when you are hit.
    void main(void){
        while(1){
            pool_receiver_timer.set(400*rtos::us);
            wait(pool_receiver_timer);
            if(get_start_bit() != -1){
                run_game.write(get_message());
                sleep(5000*rtos::ms);
            }
        }
    }
public:
    /// ir_receiver_controller
    //
    /// Parameters: one input pin, two output pins to define the ir_receiver
    /// object. The constructor initializes the timer and all the pins. The
    /// run game controller is also initialized for use with the channel.
    ///Inside the body the ground and vcc are set.
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
    
    ///Get bit
    //
    ///This function is used to get bits from the receiver.
    ///If receiving the one takes to long (> 4000 us)
    /// the function will return -1 as an integer. The
    /// function dus not need any parameters. 
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
    
    /// get_message
    //
    /// This method is used to receive the whole message. This method can only be called
    /// after the start_bit is received.
    /// Return value: char16_t number that represent the message.
    ///The function dus not need any parameters.
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
            if(i < 14){
                bitstream = bitstream << 1;
            }          
        }
        bitstream = bitstream | (1 << 15);
        hwlib::cout << bitstream << '\n';
        return bitstream;
    }
};

#endif // IR_RECEIVER_CLASSES_HPP