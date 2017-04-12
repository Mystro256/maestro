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
		charleft = al_load_bitmap("sprites/playerleft.png");
		charup   = al_load_bitmap("sprites/playerup.png");
		chardown = al_load_bitmap("sprites/playerdown.png");

		//Create Player Object with sprite
		player = new_object(
			(SCREEN_W - 16) / 2,
			(SCREEN_H - 24) / 2,
			chardown, 16, 24);
		//TODO this seems to have some issues:
		player->add_subsprite(0, 0);
		player->add_subsprite(16, 0);
		player->add_subsprite(0, 0);
		player->add_subsprite(32, 0);
	};

	~characterexample() {
		al_destroy_bitmap(charleft);
		al_destroy_bitmap(charup);
		al_destroy_bitmap(chardown);
	}

	void key_press_up() {
		player->sprite = charup;
		player->animation_rate=FPS/8;
		if(player->get_sprite_horz_flip())
			player->sprite_horz_flip();
	}

	void key_press_down() {
		player->sprite = chardown;
		player->animation_rate=FPS/8;
		if(player->get_sprite_horz_flip())
			player->sprite_horz_flip();
	}

	void key_press_left() {
		player->sprite = charleft;
		player->animation_rate=FPS/8;
		if(player->get_sprite_horz_flip())
			player->sprite_horz_flip();
	}

	void key_press_right() {
		//TODO flip me
		player->sprite = charleft;
		player->animation_rate=FPS/8;
		if(!player->get_sprite_horz_flip())
			player->sprite_horz_flip();
	}

	void key_release_up() {
		if(!key_get_hold_down() &&
		   !key_get_hold_left() &&
		   !key_get_hold_right()) {
			player->animation_rate=0;
			player->current_subsprite = 0;
		}
	}

	void key_release_down() {
		if(!key_get_hold_up() &&
		   !key_get_hold_left() &&
		   !key_get_hold_right()) {
			player->animation_rate=0;
			player->current_subsprite = 0;
		}
	}

	void key_release_left() {
		if(!key_get_hold_up() &&
		   !key_get_hold_down() &&
		   !key_get_hold_right()) {
			player->animation_rate=0;
			player->current_subsprite = 0;
		}
	}

	void key_release_right() {
		if(!key_get_hold_up() &&
		   !key_get_hold_down() &&
		   !key_get_hold_left()) {
			player->animation_rate=0;
			player->current_subsprite = 0;
		}
	}

	void key_hold_up() {
		if(player->y >= 2.0)
			player->y -= 2.0;
	}

	void key_hold_down() {
		if(player->y <= SCREEN_H - player->h - 2.0)
			player->y += 2.0;
	}

	void key_hold_left() {
		if(player->x >= 2.0)
			player->x -= 2.0;
	}

	void key_hold_right() {
		if(player->x <= SCREEN_W - player->w - 2.0)
			player->x += 2.0;
	}
};
