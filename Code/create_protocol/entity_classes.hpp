#ifndef PROTOCOL_DATA_HPP
#define PROTOCOL_DATA_HPP

class protocol_data{
	private:
		byte my_player_identifier;
		byte my_weapon_identifier;
		byte weapon_identifier;
		byte player_identifier
		bool change;
	public:
		protocol_data(byte my_player_identifier = 0, byte my_weapon_identifier = 0):
			my_player_identifier(my_id),
			my_weapon_identifier(my_weapon),
            change(false)
		{}
		byte get_player_weapon();
		byte get_my_weapon();
		byte set_player_weapon;
		byte set_player_id;
};

#endif // PROTOCOL_DATA_HPP
