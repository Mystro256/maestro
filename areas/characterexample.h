//EXAMPLE CODE//

class characterexample : area {
private:
	object* player;
	ALLEGRO_BITMAP* charleft;
	ALLEGRO_BITMAP* charup;
	ALLEGRO_BITMAP* chardown;
	int playermovex, playermovey;
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
		playermovex = 0;
		playermovey = 0;
	};

	~characterexample() {
		al_destroy_bitmap(charleft);
		al_destroy_bitmap(charup);
		al_destroy_bitmap(chardown);
	}

	void loop()
	{
		if(playermovex != 0 ||
		   playermovey != 0) {
			player->x += playermovex;
			player->y += playermovey;
			playermovex = 0;
			playermovey = 0;
			player->animation_rate=FPS/8;
		} else {
			player->animation_rate=0;
			player->current_subsprite = 0;
		}
	}

	void key_hold_up()
	{
		if(!key_get_hold_down()){
			player->sprite = charup;
			if(player->get_sprite_horz_flip())
				player->sprite_horz_flip();
			if(player->y >= 2.0)
				playermovey -= 2.0;
		}
	}

	void key_hold_down()
	{
		if(!key_get_hold_up()){
			player->sprite = chardown;
			if(player->get_sprite_horz_flip())
				player->sprite_horz_flip();
			if(player->y <= SCREEN_H - player->h - 2.0)
				playermovey += 2.0;
		}
	}

	void key_hold_left()
	{
		if(!key_get_hold_right()){
			player->sprite = charleft;
			if(player->get_sprite_horz_flip())
				player->sprite_horz_flip();
			if(player->x >= 2.0)
				playermovex -= 2.0;
		}
	}

	void key_hold_right()
	{
		if(!key_get_hold_left()){
			player->sprite = charleft;
			if(!player->get_sprite_horz_flip())
				player->sprite_horz_flip();
			if(player->x <= SCREEN_W - player->w - 2.0)
				playermovex += 2.0;
		}
	}
};
