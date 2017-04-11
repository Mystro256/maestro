//EXAMPLE CODE//

class characterexample : area {
private:
	object* player;
	ALLEGRO_BITMAP* charleft;
	ALLEGRO_BITMAP* charup;
	ALLEGRO_BITMAP* chardown;
public:
	characterexample() {
		//Initialize the area first
		init();

		//Load character sprites
		//charleft = al_load_bitmap("sprites/playerleft.png");
		//charup   = al_load_bitmap("sprites/playerup.png");
		//chardown = al_load_bitmap("sprites/playerdown.png");
		//TODO character sprites don't load, I'll have to debug later
		charleft = al_create_bitmap(48, 24);
		al_set_target_bitmap(charleft);
		al_clear_to_color(al_map_rgb(0, 0, 255));
		charup = al_create_bitmap(48, 24);
		al_set_target_bitmap(charup);
		al_clear_to_color(al_map_rgb(255, 0, 0));
		chardown = al_create_bitmap(48, 24);
		al_set_target_bitmap(chardown);
		al_clear_to_color(al_map_rgb(255, 0, 255));

		//Create Player Object with sprite
		player = new_object(
			(SCREEN_W - 16) / 2,
			(SCREEN_H - 24) / 2,
			chardown, 16, 24);
		//TODO this seems to have some issues:
		//player->add_subsprite(0, 0);
	};

	~characterexample() {
		al_destroy_bitmap(charleft);
		al_destroy_bitmap(charup);
		al_destroy_bitmap(chardown);
	}

	void key_press_up() {
		player->sprite = charup;
	}

	void key_press_down() {
		player->sprite = chardown;
	}

	void key_press_left() {
		player->sprite = charleft;
	}

	void key_press_right() {
		//TODO flip me
		player->sprite = charleft;
	}

	void key_hold_up() {
		if(player->y >= 4.0)
			player->y -= 4.0;
	}

	void key_hold_down() {
		if(player->y <= SCREEN_H - player->h - 4.0)
			player->y += 4.0;
	}

	void key_hold_left() {
		if(player->x >= 4.0)
			player->x -= 4.0;
	}

	void key_hold_right() {
		if(player->x <= SCREEN_W - player->w - 4.0)
			player->x += 4.0;
	}
};
