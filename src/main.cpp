#include "window_manager.hpp"
#include "player.hpp"

#undef main

int main()
{
	auto wm = std::make_shared<WindowManager>();
	wm->LoadXMLResources();
	wm->ScreenLoop();

	/*
	PlayerPtr player( new Human() );
	PlayerPtr monster( new Monster() );

	// fight
	bool game_over = false;
	bool is_win = false;
	while( !game_over )
	{
		player->Attack( monster );
		if ( monster->hitpoints <= 0 )
		{
			game_over = true;
			is_win = true;
		}

		monster->Attack( player );
		if ( player->hitpoints <= 0 )
		{
			game_over = true;
			is_win = false;
		}

		std::cout << ">>>>>>>>>> player hitpoints: " << player->hitpoints << ", monster hitpoints: " << monster->hitpoints << std::endl;
	}

	if ( is_win )
		std::cout << "Congratulation! You win the game!" << std::endl;
	else
		std::cout << "You lose, try again another time" << std::endl;
	*/

	return 0;
}
