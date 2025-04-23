#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <functional>
#include "exception.h"

class Window
{
private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool execute = true;
  vector<Exception> exceptionStack;

public:
  Window(const char* title = "", const int width = 1280, const int height = 720, uint32_t flags = SDL_WINDOW_SHOWN);
  ~Window();
  Window(const Window&) = default;
  Window(Window&&) = delete;
  Window& operator=(const Window&) = default;
  Window& operator=(Window&&) = delete;
  SDL_Renderer* getRenderer(SDL_Renderer* returnval = nullptr);
  SDL_Window* getWindow(SDL_Window* returnval = nullptr);
  const bool isRunning(SDL_Event* event, bool* returnval = nullptr);
  void replaceRenderer(SDL_Renderer* newRenderer);
  void replaceWindow(SDL_Window* newWindow);
  void swapExecution();
  void setExecution(const bool state);
  void endExecution();
  void changeTitle(const char* newTitle);
  void changeResolution(const int width = 0, const int height = 0);
  void updateFlags(uint32_t flags);
  vector<Exception> errorStack(vector<Exception>* stack = nullptr);
  void flushErrorStack();
};

class Keybind {
private:
  vector<SDL_KeyCode> keybind;
  function<void(vector<void*>&)>
    held, // What to do when the keybind is pushed down
    released; // What to do when the keybind is released
  vector<void*> // Extra arguments to provide the functions
    heldInputs,
    releasedInputs;
  bool active;

public:
  Keybind(vector<SDL_KeyCode> binding, function<void(vector<void*>&)> heldEvent = nullptr, function<void(vector<void*>&)> releasedEvent = nullptr, vector<void*> heldThrough = {}, vector<void*> releasedThrough = {}) {
    keybind = binding;
    held = heldEvent;
    released = releasedEvent;
    heldInputs = heldThrough;
    releasedInputs = releasedThrough;
  }
};

class KeyboardEventHandler
{
private:
};

class MouseInteractable {
private:
  function<void(vector<void*>&)> // What to do when the left/middle/right mouse button is pressed/released
    lhFunc, // Left Hold
    rhFunc, // Right Hold
    mhFunc, // Middle Hold
    lcFunc, // Left Click
    rcFunc, // Right Click
    mcFunc; // Middle Click
  SDL_Rect* collision;
  vector<Exception> exceptionStack;

public:
  MouseInteractable(const MouseInteractable&) = default;
  MouseInteractable(MouseInteractable&&) = default;
  MouseInteractable& operator=(const MouseInteractable&) = default;
  MouseInteractable& operator=(MouseInteractable&&) = default;
  MouseInteractable(
    SDL_Rect* collisionbox,
    const function<void(vector<void*>&)> leftclick = nullptr,
    const function<void(vector<void*>&)> rightclick = nullptr,
    const function<void(vector<void*>&)> middleclick = nullptr,
    const function<void(vector<void*>&)> lefthold = nullptr,
    const function<void(vector<void*>&)> righthold = nullptr,
    const function<void(vector<void*>&)> middlehold = nullptr);

  SDL_Rect* getCollisionBox(SDL_Rect* returnval = nullptr);
  function<void(vector<void*>&)> getLeftClickFunction(function<void(vector<void*>&)> returnval = nullptr);
  function<void(vector<void*>&)> getRightClickFunction(function<void(vector<void*>&)> returnval = nullptr);
  function<void(vector<void*>&)> getMiddleClickFunction(function<void(vector<void*>&)> returnval = nullptr);
  function<void(vector<void*>&)> getLeftHoldFunction(function<void(vector<void*>&)> returnval = nullptr);
  function<void(vector<void*>&)> getRightHoldFunction(function<void(vector<void*>&)> returnval = nullptr);
  function<void(vector<void*>&)> getMiddleHoldFunction(function<void(vector<void*>&)> returnval = nullptr);

  vector<Exception> errorStack(vector<Exception>* errorStack = nullptr);
  void flushErrorStack();
};

class StaticMouseInteractables { // Static interactables are more performant than dynamic interactables. O(log(n)) performance

};

class DynamicMouseInteractables { // Mouse interactables that can be moved and changed around. O(n) performance

};

class MouseEventHandler {};

class ControllerEventHandler // I'll implement this at some point in the "near" future
{
  // The chances of this being implemented in a reasonable time is highly unlikely
};

#endif