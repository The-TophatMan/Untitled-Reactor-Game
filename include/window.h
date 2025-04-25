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
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool execute = true;
  vector<Exception> exceptionStack;

public:
  Window(const char *title = "", const int width = 1280, const int height = 720, uint32_t flags = SDL_WINDOW_SHOWN);
  ~Window();
  Window(const Window &) = default;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = default;
  Window &operator=(Window &&) = delete;
  SDL_Renderer *getRenderer(SDL_Renderer *returnval = nullptr);
  SDL_Window *getWindow(SDL_Window *returnval = nullptr);
  const bool isRunning(SDL_Event *event, bool *returnval = nullptr);
  void replaceRenderer(SDL_Renderer *newRenderer);
  void replaceWindow(SDL_Window *newWindow);
  void swapExecution();
  void setExecution(const bool state);
  void endExecution();
  void changeTitle(const char *newTitle);
  void changeResolution(const int width = 0, const int height = 0);
  void updateFlags(uint32_t flags);
  vector<Exception> errorStack(vector<Exception> *stack = nullptr);
  void flushErrorStack();
};

class Keybind
{
private:
  vector<SDL_KeyCode> keybind;
  function<void(vector<void *> &)>
      held,      // What to do when the keybind is pushed down
      released;  // What to do when the keybind is released
  vector<void *> // Extra arguments to provide the functions
      heldInputs,
      releasedInputs;
  bool active;
  vector<Exception> exceptionStack;

public:
  Keybind(const Keybind &) = default;
  Keybind(Keybind &&) = default;
  Keybind &operator=(const Keybind &) = default;
  Keybind &operator=(Keybind &&) = default;
  Keybind(vector<SDL_KeyCode> binding,
          function<void(vector<void *> &)> heldEvent = nullptr,
          function<void(vector<void *> &)> releasedEvent = nullptr,
          vector<void *> heldElements = {},
          vector<void *> releasedElements = {});
  Keybind(SDL_KeyCode key, function<void(vector<void *> &)> heldEvent = nullptr,
          vector<void *> heldElements = {},
          function<void(vector<void *> &)> releasedEvent = nullptr,
          vector<void *> releasedElements = {});

  const function<void(vector<void *> &)> getHoldEvent(function<void(vector<void *> &)> *returnval = nullptr);
  const function<void(vector<void *> &)> getReleasedEvent(function<void(vector<void *> &)> *returnval = nullptr);
  const vector<void *> getHeldElements(vector<void *> *returnval = nullptr);
  const vector<void *> getReleasedElements(vector<void *> *returnval = nullptr);
  const bool isActive(bool *returnval = nullptr);
  const vector<SDL_KeyCode> getKeybind(vector<SDL_KeyCode> *returnval = nullptr);

  void replaceHoldEvent(function<void(vector<void *> &)> newEvent,
                        vector<void *> holdElements);
  void replaceReleasedEvent(function<void(vector<void *> &)> newEvent,
                            vector<void *> releasedElements);
  void toggleActive();
  void setActive(const bool set);
  void deactivate();
  void activate();

  void removeHoldEvent();
  void removeReleasedEvent();

  vector<Exception> errorStack(vector<Exception> *retrunval = nullptr);
  void flushErrorStack();
};

class KeyboardEventHandler
{
private:
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
  vector<Exception> exceptionStack;

public:
  MouseInteractable(const MouseInteractable &) = default;
  MouseInteractable(MouseInteractable &&) = default;
  MouseInteractable &operator=(const MouseInteractable &) = default;
  MouseInteractable &operator=(MouseInteractable &&) = default;
  MouseInteractable(
      SDL_Rect *collisionbox,
      const function<void(vector<void *> &)> leftclick = nullptr, const vector<void *> leftclickElements = {},
      const function<void(vector<void *> &)> rightclick = nullptr, const vector<void *> rightclickElements = {},
      const function<void(vector<void *> &)> middleclick = nullptr, const vector<void *> middleclickElements = {},
      const function<void(vector<void *> &)> lefthold = nullptr, const vector<void *> leftholdElements = {},
      const function<void(vector<void *> &)> righthold = nullptr, const vector<void *> rightholdElements = {},
      const function<void(vector<void *> &)> middlehold = nullptr, const vector<void *> middleholdElements = {})
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
      exceptionStack.push_back(Exception("Failed to initialize collision box",
                                         "MouseInteractable::MouseInteractable",
                                         -1, ERROR));
      return;
    }
    collision = collisionbox;
    if (collision != collisionbox)
      exceptionStack.push_back(Exception("Failed to save collision box",
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

  vector<Exception> errorStack(vector<Exception> *errorStack = nullptr);
  void flushErrorStack();
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