#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "exception"

/// @brief A basic window class for constructing windows with SDL2.
class Window
{
private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool execute = true;

protected:
public:
  /// @brief The basic window constructor
  /// @param title The title of the window
  /// @param width The width of the window in pixels. Defaults to 1280
  /// @param height The height of the window in pixels. Defaults to 720
  Window(const char* title, const int width = 1280, const int height = 720);
  ~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }

  /// @brief Get the window renderer. Can grab by passing through a variable or by getting the return value.
  /// @param returnval The variable to pass through to grab the renderer. Defaults to nullptr
  /// @return The pointer return value
  SDL_Renderer* getRenderer(SDL_Renderer* returnval = nullptr) {
    if (returnval != nullptr) returnval = renderer;
    return renderer;
  }

  /// @brief Get the window object from SDL. Can grab by passing through a variable or by getting the return value.
  /// @param returnval The variable to pass through to grab the renderer. Defaults to nullptr
  /// @return A pointer to the window
  SDL_Window* getWindow(SDL_Window* returnval = nullptr) {
    if (returnval != nullptr) returnval = window;
    return window;
  }

  const bool isRunning(bool* returnval = nullptr) {
    if (returnval != nullptr) *returnval = execute;
    return execute;
  }
};
#endif