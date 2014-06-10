#include <iostream>
#include "window_manager.hpp"
#include "cross_zero.hpp"

WindowManager::WindowManager()
{
	Init();
}

WindowManager::~WindowManager()
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

int WindowManager::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	return 0;
}

int WindowManager::LoadTextures()
{
	std::string texture_files[3] = {
		"/home/ilya/test_dev/x_proj/res/empty.bmp",
		"/home/ilya/test_dev/x_proj/res/zero.bmp",
		"/home/ilya/test_dev/x_proj/res/cross.bmp"
	};

	SDL_Surface* bmp;
	for( int i = -1; i < 2; ++i )
	{
		bmp = SDL_LoadBMP(texture_files[ i + 1 ].c_str());
		if (bmp == nullptr){
			std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
			return 1;
		}

		textures.insert( std::make_pair( i, SDL_CreateTextureFromSurface(ren, bmp) ) );
		SDL_FreeSurface(bmp);
		if (textures[ i ] == nullptr){
			std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return 1;
		}
	}

	return 0;
}

void WindowManager::ScreenLoop()
{
	bool done = false;
	SDL_Event event;

	Field field(3, 3);

	bool cross = true;
	while (!done)
	{
		SDL_WaitEvent(&event);
		switch( event.type )
		{
			case SDL_KEYDOWN:
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE: // Выход из программы по нажатию Esc
							{
								done = true;
								break;
							}
					}
					break;
				}
			case SDL_MOUSEBUTTONDOWN:
				{
					switch(event.button.button)
					{
						case SDL_BUTTON_LEFT:
							{
								std::cout << "x = " << event.button.x << ", y = " << event.button.y << std::endl;
								field.SetCell( event.button.x / 214, event.button.y / 160, (cross ? Type::CROSS : Type::ZERO) );
								cross = !cross;
								break;
							}
					}
				}
		}

		SDL_RenderClear(ren);
		field.PrintField( std::bind( &WindowManager::PrintTexture,
										this,
										std::placeholders::_1,
										std::placeholders::_2,
										std::placeholders::_3
									)
						);

		SDL_RenderPresent(ren);

		int winner = field.CheckWinner();

		if( winner >= 0 )
		{
			std::cout << "player" << winner << " wins!" << std::endl;
			done = true;
			break;
		}

		if( field.IsEndGame() )
		{
			std::cout << "it's a tie!" << std::endl;
			done = true;
		}
	}
}

void WindowManager::PrintTexture( int x, int y, int type )
{
	SDL_Rect dstRect;
	dstRect.x = 214 * x;
	dstRect.y = 160 * y;
	dstRect.w = 214;
	dstRect.h = 160;

	SDL_RenderCopy( ren, textures[ type ], NULL, &dstRect );
}
