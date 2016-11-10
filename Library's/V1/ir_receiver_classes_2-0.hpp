#include "hwlib.hpp"
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
    
    /// get_start_bit
    //
    /// This method is used to receive the first bit of a message
    /// Return value: This function returns a 1 or 0 depending on 
    /// the value of the bit send. If there is no bit send it returns
    /// -1.
    int get_start_bit()
    {
         if(!r_pin.get()){
            hwlib::wait_us(1000);
            if(!r_pin.get()){
               hwlib::wait_us(700);
                return 1;
            }else{
                return 0;
            }
        }
        return -1;
    }
    
    /// get_bit
    //
    /// This method tries to get a bit. When this doesn't happen within 4 ms 
    /// the method will return -1, otherwise it will return the received bit
    int get_bit(){
        int bit;
        auto begin_time = hwlib::now_us();
        while((hwlib::now_us() - begin_time) < 4000){
            bit = get_start_bit();
            if(bit != -1){
                return bit;
            }
        }
        return -1;
    }
    
    /// get_message
    //
    /// This method is used to receive the whole message. This method can only be called
    /// after the start_bit is received.
    /// Return value: char16_t number that represent the message 
    char16_t get_message(){
        char16_t bitstream = 1;
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
        return bitstream;
    }
    
};

/// ir_receiver_controller
//
/// This class adds extra functionality to ir_receiver
class ir_receiver_controller{
private:
    ir_receiver receiver;
public:
    /// ir_receiver_controller
    //
    /// Parameters: one input pin, two output pins to define the ir_receiver
    /// object
    ir_receiver_controller(hwlib::pin_in & r_pin, hwlib::pin_out & gnd, hwlib::pin_out &vcc):
    receiver(r_pin,gnd,vcc)
    {}
    
    /// recieve_full_message
    //
    /// This method receives the full message from start_bit to printing the message on the screen
    /// Return value: 1 when succesful, 0 when unsuccesfull
    int receive_full_message(){
        while(1){
        auto bit = receiver.get_start_bit();
        while(1){
            if(bit == 1){
                break;
            }else{
                bit = receiver.get_bit();
            }
        }
        auto byte = receiver.get_message();
        if(byte == -1){
            return 0;
        }else{
            hwlib::cout << byte << '\n';
            hwlib::wait_ms(400);
            return 1;
        }
        }
    }

};




