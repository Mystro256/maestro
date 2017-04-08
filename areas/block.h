//EXAMPLE CODE//

const int SPRITE_SIZE = 32;

class block : area {
private:
	object* player;
	ALLEGRO_BITMAP* blocksprite;
public:
	block() {
		//Initialize the area first
		init();

		//Create block sprite
		blocksprite = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);
		al_set_target_bitmap(blocksprite);
		al_clear_to_color(al_map_rgb(255, 0, 255));

		//Create Player Object with sprite
		player = new_object(
			(SCREEN_W - SPRITE_SIZE) / 2,
			(SCREEN_H - SPRITE_SIZE) / 2,
			blocksprite);
	};

	~block() {
		al_destroy_bitmap(blocksprite);
	}

	void key_hold_up() {
		if(player->y >= 4.0)
			player->y -= 4.0;
	}

	void key_hold_down() {
		if(player->y <= get_screen_h() - player->bh - 4.0)
			player->y += 4.0;
	}

	void key_hold_left() {
		if(player->x >= 4.0)
			player->x -= 4.0;
	}

	void key_hold_right() {
		if(player->x <= get_screen_w() - player->bw - 4.0)
			player->x += 4.0;
	}
};
