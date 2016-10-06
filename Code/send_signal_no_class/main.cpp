#include "hwlib.hpp"

void send_one(hwlib::target::d2_36kHz  ir){
  ir.set(1);
  hwlib::wait_us(1600);
  ir.set(0);
  hwlib::wait_us(800);
}
void send_zero(hwlib::target::d2_36kHz  ir){
  ir.set(1);
  hwlib::wait_us(800);
  ir.set(0);
  hwlib::wait_us(1600);
}

void Send_B(hwlib::target::d2_36kHz  ir){
  send_zero(ir);
  hwlib::wait_us(1);
  send_one(ir);
  hwlib::wait_us(1);
  send_zero(ir);
  hwlib::wait_us(1);
  send_zero(ir);
  hwlib::wait_us(1);
  send_zero(ir);
  hwlib::wait_us(1);
  send_zero(ir);
  hwlib::wait_us(1);
  send_one(ir);
  hwlib::wait_us(1);
  send_zero(ir);
  hwlib::wait_us(1);
}

int encoder(int identifier, int weapon)
{
    int sending_bit = 1;
    sending_bit = sending_bit << 5;
    sending_bit = (sending_bit | identifier);
    sending_bit = sending_bit << 5;
    sending_bit = (sending_bit | weapon);
    sending_bit = sending_bit << 5;
    sending_bit = (sending_bit | (identifier ^ weapon));
    return sending_bit;
}

void decoder(int receiving_bit)
{
    int identifier_player= 0;
    int identifier_weapon= 0;
    int exor = 0;
    //Getting identifier
    for(int i = 14; i >= 10; i--){
        if((receiving_bit & (1 << i)) != 0){
            identifier_player = (identifier_player| 1);
        }else{
            identifier_player = (identifier_player | 0);
        }
        if(i != 10){
            identifier_player = identifier_player << 1;
        }
    }
    for(int i = 9; i >= 5; i--){
        if((receiving_bit & (1 << i)) != 0){
            identifier_weapon = (identifier_weapon |1);
        }else{
            identifier_weapon = (identifier_weapon | 0);
        }
        if(i != 5){
            identifier_weapon = identifier_weapon << 1;
        }
    }  
    for(int i= 4; i >= 0; i--){
        if((receiving_bit & (1 << i)) != 0){
            exor = (exor | 1);
        }else{
            exor = (exor | 0);
        }
        if(i != 0){
            exor = exor<< 1;
        }
    }
    
}

int main(){
 //Killing watchdog and waiting for commandprompt to open
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    
    //Defining pins
    auto ir = hwlib::target::d2_36kHz();
    auto button = hwlib::target::pin_in(hwlib::target::pins::d7);

    //Running program
    for(;;){
        hwlib::wait_ms( 1 ); 
        if (! button.get() == 1){
            hwlib::cout << "Sending byte \n";
            Send_B(ir);
            hwlib::wait_ms(1000);
        }     
        hwlib::wait_ms( 1 );
        ir.set( 0 );
   }    
}