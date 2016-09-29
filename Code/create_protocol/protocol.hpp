#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include "entity_classes.hpp"

///Abstract protocol class
//
///Abstract interface class used for easy access to the protocol sub classes.
class protocol{
	public:
		virtual char16_t read() = 0;
		virtual void write() = 0;
};


class send_protocol : public protocol{
	private:
		protocol_data & my_player;
	public:
		send_protocol(protocol_data & my_player):
		my_player(my_player)
		{}
		void write();
		char16_t encode(){
			myweapon = myplayer.getWeapon();
		};
}

class receive_protocol : public protocol{
	private:
		protocol_data & my_pl;
	public:
		send_protocol(protocol_data & my_player):
		my_player(my_player)
		{}
		void write();
		char16_t decode(){
			myweapon = myplayer.getWeapon();
		};
}
#endif // PROTOCOL_HPP
