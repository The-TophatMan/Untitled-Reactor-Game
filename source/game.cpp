#include "../include/game.h"
#include "../include/exception.h"
#include <SDL2/SDL_render.h>

Window::Window(const char* title, const int width, const int height) {
  try {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) throw(Exception("Failed to create window", "Window", 1, 2));
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) throw(Exception("Failed to create renderer", "Window", 1, 2));
  }
  catch (Exception& err) {
    err << "Window";
    throw err;
  }
}

Window::~Window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

SDL_Renderer* Window::getRenderer(SDL_Renderer* returnval) {
  if (returnval != nullptr)
    returnval = renderer;
  return renderer;
}

SDL_Window* Window::getWindow(SDL_Window* returnval) {
  if (returnval != nullptr)
    returnval = window;
  return window;
}

const bool Window::isRunning(bool* returnval) {
  if (returnval != nullptr)
    *returnval = execute;
  return execute;
}

const bool Window::replaceRenderer(SDL_Renderer* newRenderer)
{
  SDL_DestroyRenderer(renderer);
  if (renderer) throw Exception("Failed to destroy previous renderer", "Window::replaceRenderer", -1, 1);
  renderer = newRenderer;
  if (!renderer) throw Exception("Failed to replace renderer pointer", "Window::replaceRenderer", -1, 1);
  return true;
}

const bool Window::replaceWindow(SDL_Window* newWindow) {
  SDL_DestroyWindow(window);
  if (window)
    throw Exception("Failed to destroy window", "Window::replaceWindow", -1, 1);
  window = newWindow;
  if (!window)
    throw Exception("Failed to replace window pointer", "Window::replaceWindow",
      -1, 1);
  return true;
}

void Window::swapExecution() { execute = !execute; }

void Window::setExecution(const bool state) { execute = state; }

void Window::endExecution() { execute = false; }

