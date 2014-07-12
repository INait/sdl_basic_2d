#include <iostream>
#include "window_manager.hpp"
#include "cross_zero.hpp"

#include "../3rd/tinyxml/tinyxml.h"

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

void WindowManager::LoadXMLResources()
{
	TiXmlDocument doc("../res/res.xml");
	if (!doc.LoadFile())
	{
		std::cout << "xml wasn't loaded" << std::endl;
		return;
	}
	TiXmlNode* pResElem;

	pResElem = doc.FirstChild("resources");
	if (!pResElem)
	{
		std::cout << "wrong XML format: no 'resources' node" << std::endl;
		return;
	}

	// check if set param exists
	TiXmlElement* pSetElem = pResElem->FirstChildElement("set");
	std::string resPrefix = (pSetElem && pSetElem->Attribute("path")) ? pSetElem->Attribute("path") : "";

	/*
	for (TiXmlElement* pFontElem = pResElem->FirstChildElement("font"); pFontElem; pFontElem = pFontElem->NextSiblingElement("font"))
	{
		std::string name = pFontElem->Attribute("name");
		std::string file = pFontElem->Attribute("file");

		fonts.insert(std::make_pair(name, file));
	}
	*/

	TiXmlElement* pAtlasElem = pResElem->FirstChildElement("atlas");
	if (pAtlasElem)
	{
		try {
			int i = -2;
			for (TiXmlElement* pTextureElem = pAtlasElem->FirstChildElement("image"); pTextureElem; pTextureElem = pTextureElem->NextSiblingElement("image"))
			{
				std::string name = pTextureElem->Attribute("name");
				std::string file = resPrefix + pTextureElem->Attribute("file");
				SDL_Texture* texture = LoadImage(file);

				textures.insert(std::make_pair(i, texture));
				i++;
			}
		}
		catch (const std::runtime_error &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
	}

	SDL_Surface* icon = IMG_Load("../res/icon.ico");
	if (!icon)
	{
		std::cout << "Error loading window icon" << std::endl;
	}
	SDL_SetWindowIcon(win, icon);
}

SDL_Texture* WindowManager::LoadImage(std::string file)
{
	SDL_Surface* surf = nullptr;
	surf = IMG_Load(file.c_str());
	if (surf == nullptr)
		throw std::runtime_error("Failed to load image: " + file + IMG_GetError());

	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
	return tex;
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
