#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <functional>
#include "exception.h"

class GUIInteractables
{
private:
  function<void(vector<void*>&)> func;
  bool mouseState;
  GUIInteractables* left = nullptr, * right = nullptr, * up = nullptr, * down = nullptr;
  SDL_Rect* rectangle = nullptr;
  bool active = true, stackActive = true;
  vector<Exception*> exceptionStack;

public:
  GUIInteractables(const GUIInteractables&) = default;
  GUIInteractables(GUIInteractables&&) = default;
  GUIInteractables& operator=(const GUIInteractables&) = default;
  GUIInteractables& operator=(GUIInteractables&&) = default;
  GUIInteractables(SDL_Rect* newRect, const function<void(vector<void*>&)>& newfunc, const bool button = 1);
  SDL_Rect* getRectangle(SDL_Rect* returnval = nullptr);
  GUIInteractables* getLeft(GUIInteractables* returnval = nullptr);
  GUIInteractables* getRight(GUIInteractables* returnval = nullptr);
  GUIInteractables* getUp(GUIInteractables* returnval = nullptr);
  GUIInteractables* getDown(GUIInteractables* returnval = nullptr);
  void addInteractable(GUIInteractables* newInteractable);
  GUIInteractables* findItem(const int x, const int y, GUIInteractables* returnval);
  bool getActive(bool* returnval = nullptr);
  void toggleActive();
  void setActive(const bool state);
  void changeFunction(const function<void(vector<void*>&)>& newfunc);
  void changeButton(const int button);
  bool getButton(bool* button = nullptr);
  function<void(vector<void*>&)> getFunction(function<void(vector<void*>&)> returnval = nullptr);
};

class KeyboardEventHandler
{
private:
  map<SDL_KeyCode, pair<function<void(vector<void*>&)>, pair<vector<void*>, bool>>> keys;

public:
  KeyboardEventHandler() {}
  KeyboardEventHandler(const KeyboardEventHandler&) = default;
  KeyboardEventHandler(KeyboardEventHandler&&) = default;
  KeyboardEventHandler& operator=(const KeyboardEventHandler&) = default;
  KeyboardEventHandler& operator=(KeyboardEventHandler&&) = default;
  void addKeyboardInput(const SDL_KeyCode key, const function<void(vector<void*>&)>& func, vector<void*> extradata = {}, const bool state = 0);
  void modifyKeyboardInput(const SDL_KeyCode key, const function<void(vector<void*>&)>& newfunc = NULL, vector<void*> extradata = {}, const int state = -1);
  void removeKeyboardInput(const SDL_KeyCode key);
  void cycleKeyboardInputs(SDL_Event* event);
};

class MouseEventHandler
{
private:
  map<SDL_Rect*, function<void(vector<void*>&)>> lHoldGameInt, lPushGameInt, rHoldGameInt, rPushGameInt, mHoldGameInt, mPushGameInt;
  map<int, GUIInteractables*> guis;

public:
  int addGUI(GUIInteractables* firstElement);
  void addGameInteractable(SDL_Rect* rectangle, const function<void(vector<void*>&)>& func, const bool mouseState = 0, const int mouseButton = 0);
  void cycleMouseEvents(SDL_Event* event);
};

class ControllerEventHandler
{
};

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

#endif