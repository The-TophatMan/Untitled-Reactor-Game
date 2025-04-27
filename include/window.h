#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <map>
#include <vector>
#include <functional>
#include "exception.h"

class Window
{
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool execute = true;
  ExceptionStack stack;

public:
  Window(const char *title = "", const int width = 1280, const int height = 720, uint32_t flags = SDL_WINDOW_SHOWN);
  ~Window();
  Window(const Window &) = default;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = default;
  Window &operator=(Window &&) = delete;
  SDL_Renderer *getRenderer(SDL_Renderer *returnval = nullptr);
  SDL_Window *getWindow(SDL_Window *returnval = nullptr);
  bool isRunning(SDL_Event *event, bool *returnval = nullptr);
  void replaceRenderer(SDL_Renderer *newRenderer);
  void replaceWindow(SDL_Window *newWindow);
  void swapExecution();
  void setExecution(const bool state);
  void endExecution();
  void changeTitle(const char *newTitle);
  void changeResolution(const int width = 0, const int height = 0);
  void updateFlags(uint32_t flags);

  ExceptionStack *errorStack(ExceptionStack *returnval = nullptr);
};

class Keybind
{
private:
  SDL_KeyCode keybind;
  function<void(vector<void *> &)>
      held,      // What to do when the keybind is pushed down
      released;  // What to do when the keybind is released
  vector<void *> // Extra arguments to provide the functions
      heldInputs,
      releasedInputs;
  bool active;
  Uint16 mod;
  ExceptionStack stack;

public:
  Keybind(const Keybind &) = default;
  Keybind(Keybind &&) = default;
  Keybind &operator=(const Keybind &) = default;
  Keybind &operator=(Keybind &&) = default;
  Keybind(const SDL_KeyCode key,
          Uint16 modifier = 0,
          function<void(vector<void *> &)> heldEvent = nullptr,
          vector<void *> heldElements = {},
          function<void(vector<void *> &)> releasedEvent = nullptr,
          vector<void *> releasedElements = {});

  function<void(vector<void *> &)> getHoldEvent(function<void(vector<void *> &)> *returnval = nullptr);
  function<void(vector<void *> &)> getReleasedEvent(function<void(vector<void *> &)> *returnval = nullptr);
  vector<void *> getHeldElements(vector<void *> *returnval = nullptr);
  vector<void *> getReleasedElements(vector<void *> *returnval = nullptr);
  bool isActive(bool *returnval = nullptr);
  SDL_KeyCode getKeybind(SDL_KeyCode *returnval = nullptr);
  Uint16 getModifier(Uint16 *returnval = nullptr);

  void replaceHoldEvent(function<void(vector<void *> &)> newEvent,
                        vector<void *> holdElements);
  void replaceReleasedEvent(function<void(vector<void *> &)> newEvent,
                            vector<void *> releasedElements);
  void replaceModifier(const Uint16 newModifier);
  void toggleActive();
  void setActive(const bool set);
  void deactivate();
  void activate();

  void removeHoldEvent();
  void removeReleasedEvent();
  void removeModifier();

  ExceptionStack *errorStack(ExceptionStack *retrunval = nullptr);
};

class KeyboardEventHandler
{
private:
  map<pair<SDL_KeyCode, Uint16>, Keybind *> keybinds;
  ExceptionStack stack;

public:
  KeyboardEventHandler(const KeyboardEventHandler &) = default;
  KeyboardEventHandler(KeyboardEventHandler &&) = default;
  KeyboardEventHandler &operator=(const KeyboardEventHandler &) = default;
  KeyboardEventHandler &operator=(KeyboardEventHandler &&) = default;
  KeyboardEventHandler(map<pair<SDL_KeyCode, Uint16>, Keybind *> import = {});

  void addKeybind(Keybind *keybind);
  Keybind *getKeybind(SDL_KeyCode key, Uint16 modifier = 0, Keybind *returnval = nullptr);
  Keybind *getKeybind(Keybind *bind, Keybind *returnval = nullptr);
  void removeKeybind(SDL_KeyCode key, Uint16 modifier = 0);
  void removeKeybind(Keybind *bind);

  void cycleEvents(SDL_Event *event)
  {
    while (SDL_PollEvent(event))
    {
    }
  }

  ExceptionStack *errorStack(ExceptionStack *retrunval = nullptr);
};

class MouseInteractable
{
private:
  function<void(vector<void *> &)> // What to do when the left/middle/right mouse button is pressed/released
      lhFunc,                      // Left Hold
      rhFunc,                      // Right Hold
      mhFunc,                      // Middle Hold
      lcFunc,                      // Left Click
      rcFunc,                      // Right Click
      mcFunc;                      // Middle Click
  vector<void *>                   // Arguments to pass into each of the functions
      lhPass,
      rhPass,
      mhPass,
      lcPass,
      rcPass,
      mcPass;
  SDL_Rect *collision;
  ExceptionStack stack;

public:
  MouseInteractable(const MouseInteractable &) = default;
  MouseInteractable(MouseInteractable &&) = default;
  MouseInteractable &operator=(const MouseInteractable &) = default;
  MouseInteractable &operator=(MouseInteractable &&) = default;
  MouseInteractable(
      SDL_Rect *collisionbox,
      function<void(vector<void *> &)> leftclick = nullptr, const vector<void *> leftclickElements = {},
      function<void(vector<void *> &)> rightclick = nullptr, const vector<void *> rightclickElements = {},
      function<void(vector<void *> &)> middleclick = nullptr, const vector<void *> middleclickElements = {},
      function<void(vector<void *> &)> lefthold = nullptr, const vector<void *> leftholdElements = {},
      function<void(vector<void *> &)> righthold = nullptr, const vector<void *> rightholdElements = {},
      function<void(vector<void *> &)> middlehold = nullptr, const vector<void *> middleholdElements = {})
  {
    lhFunc = lefthold;
    rhFunc = righthold;
    mhFunc = middlehold;
    lcFunc = leftclick;
    rcFunc = rightclick;
    mcFunc = middleclick;
    lhPass = leftholdElements;
    rhPass = rightholdElements;
    mhPass = middleholdElements;
    lcPass = leftclickElements;
    rcPass = rightclickElements;
    mcPass = middleclickElements;
    if (!collisionbox)
    {
      stack.insert(Exception("Failed to initialize collision box",
                             "MouseInteractable::MouseInteractable",
                             -1, ERROR));
      return;
    }
    collision = collisionbox;
    if (collision != collisionbox)
      stack.insert(Exception("Failed to save collision box",
                             "MouseInteractable::MouseInteractable",
                             -1, ERROR));
  }

  SDL_Rect *getCollisionBox(SDL_Rect *returnval = nullptr);
  function<void(vector<void *> &)> getLeftClickFunction(function<void(vector<void *> &)> returnval = nullptr);
  function<void(vector<void *> &)> getRightClickFunction(function<void(vector<void *> &)> returnval = nullptr);
  function<void(vector<void *> &)> getMiddleClickFunction(function<void(vector<void *> &)> returnval = nullptr);
  function<void(vector<void *> &)> getLeftHoldFunction(function<void(vector<void *> &)> returnval = nullptr);
  function<void(vector<void *> &)> getRightHoldFunction(function<void(vector<void *> &)> returnval = nullptr);
  function<void(vector<void *> &)> getMiddleHoldFunction(function<void(vector<void *> &)> returnval = nullptr);

  ExceptionStack *errorStack(ExceptionStack *errorStack = nullptr);
};

class StaticMouseInteractables
{ // Static interactables are more performant than dynamic interactables. ~O(log(n)) performance
};

class DynamicMouseInteractables
{ // Mouse interactables that can be moved and changed around. O(n) performance
};

class MouseEventHandler
{
};

class ControllerEventHandler // I'll implement this at some point in the "near" future
{
  // The chances of this being implemented in a reasonable time is highly unlikely
};

#endif