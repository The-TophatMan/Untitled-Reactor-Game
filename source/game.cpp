#include "../include/game.h"
#include "../include/exception.h"
#include <SDL2/SDL_render.h>

/// @brief The basic window constructor
/// @param title The title of the window
/// @param width The width of the window in pixels. Defaults to 1280
/// @param height The height of the window in pixels. Defaults to 720
Window::Window(const char* title, const int width, const int height) {
  try {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) throw(Exception("Failed to create window", "Window", 1, 3));
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) throw(Exception("Failed to create renderer", "Window", 1, 3));
  }
  catch (Exception& err) {
    err << "Window";
    throw err;
  }
}