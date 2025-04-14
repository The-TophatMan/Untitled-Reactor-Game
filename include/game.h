#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

class Game
{
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool execute = true;

protected:
public:
};
#endif