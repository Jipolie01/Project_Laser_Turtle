#ifndef PROTOCOL_DATA_HPP
#define PROTOCOL_DATA_HPP

class protocol_data{
	private:
		byte my_id;
		byte my_weapon;
		byte id;
		byte weapon
		bool change = false;
	public:
		protocol_data(byte my_id, byte my_weapon):
			my_id(my_id),
			my_weapon(my_weapon)
		{}
		byte get_player_weapon();
		byte get_my_weapon();
		byte set_player_weapon;
		byte set_player_id;
};

#endif // PROTOCOL_DATA_HPP
