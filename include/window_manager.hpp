#include <memory>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef _WINDOW_MANAGER_HPP_
#define _WINDOW_MANAGER_HPP_

class WindowManager
{
public:
	WindowManager();

	int Init();

	void LoadXMLResources();
	
	SDL_Texture* LoadImage(std::string file);

	void ScreenLoop();

	void PrintTexture(int x, int y, int type);

	~WindowManager();

private:
	SDL_Window* win;
	SDL_Renderer* ren;

	std::map< int, SDL_Texture* > textures;
};

#endif // _WINDOW_MANAGER_HPP_
