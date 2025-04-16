#include "../include/window.h"
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
  try {
    SDL_Renderer* temp = renderer;
    if (temp != renderer || !temp) throw Exception("Failed to save old renderer to a temporary location", "replaceRenderer", -1, 1);
    renderer = newRenderer;
    if (renderer != newRenderer || !renderer) {
      renderer = temp;
      if (renderer != temp || !renderer) throw Exception("Failed to overwrite old renderer with new one. Failed to return old renderer to previous state", "replaceRenderer", -1, 1);
      throw Exception("Failed to overwrite old renderer with new one. Old renderer has been preserved", "replaceRenderer", -1, 0);
    }
    SDL_DestroyRenderer(temp);
    if (temp) throw Exception("Failed to destroy old renderer. This may cause memory leaks", "replaceRenderer", -1, 0);
  }
  catch (Exception& err) {
    err << "Window";
    throw err;
  }
  return true;
}

const bool Window::replaceWindow(SDL_Window* newWindow)
{
  try {
    SDL_Window* temp = window;
    if (temp != window || !temp) throw Exception("Failed to save old window to a temporary location", "replaceWindow", -1, 1);
    window = newWindow;
    if (window != newWindow || !window) {
      window = temp;
      if (window != temp || !window) throw Exception("Failed to overwrite old window with new one. Failed to return old window to previous state", "replaceWindow", -1, 1);
      throw Exception("Failed to overwrie old window with new one. Old window has been preserved", "replaceWindow", -1, 0);
    }
    SDL_DestroyWindow(temp);
    if (temp) throw Exception("Failed to destroy old window. This may cause memory leaks", "replaceWindow", -1, 0);
  }
  catch (Exception& err) {
    err << "Window";
    throw err;
  }
  return true;
}

void Window::swapExecution() { execute = !execute; }

void Window::setExecution(const bool state) { execute = state; }

void Window::endExecution() { execute = false; }

