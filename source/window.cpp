#include "../include/window.h"
#include "../include/exception.h"
#include <SDL2/SDL_render.h>

// Window implementation
Window::Window(const char *title, const int width, const int height, uint32_t flags)
{
  try
  {
    if (width <= 0 || height <= 0)
      throw Exception("Provided width or height is too small", "Window", -1, 2);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window)
      throw Exception("Failed to create window", "Window", 1, 2);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
      throw Exception("Failed to create renderer", "Window", 1, 2);
  }
  catch (Exception &err)
  {
    err << "Window";
    throw err;
  }
}

Window::~Window()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

SDL_Renderer *Window::getRenderer(SDL_Renderer *returnval)
{
  if (returnval != nullptr)
    returnval = renderer;
  return renderer;
}

SDL_Window *Window::getWindow(SDL_Window *returnval)
{
  if (returnval != nullptr)
    returnval = window;
  return window;
}

const bool Window::isRunning(bool *returnval)
{
  if (returnval != nullptr)
    *returnval = execute;
  return execute;
}

void Window::replaceRenderer(SDL_Renderer *newRenderer)
{
  try
  {
    if (!newRenderer)
      throw Exception("New Renderer did not properly initialize", "replaceRenderer", -1, 2);
    SDL_Renderer *temp = renderer;
    if (temp != renderer || !temp)
      throw Exception("Failed to save old renderer to a temporary location", "replaceRenderer", -1, 2);
    renderer = newRenderer;
    if (renderer != newRenderer || !renderer)
    {
      renderer = temp;
      if (renderer != temp || !renderer)
        throw Exception("Failed to overwrite old renderer with new one. Failed to return old renderer to previous state", "replaceRenderer", -1, 2);
      throw Exception("Failed to overwrite old renderer with new one. Old renderer has been preserved", "replaceRenderer", -1, 1);
    }
    SDL_DestroyRenderer(temp);
    if (temp)
      throw Exception("Failed to destroy old renderer. This may cause memory leaks", "replaceRenderer", -1, 1);
  }
  catch (Exception &err)
  {
    err << "Window";
    throw err;
  }
}

void Window::replaceWindow(SDL_Window *newWindow)
{
  try
  {
    if (!newWindow)
      throw Exception("New window did not properly initialize", "replaceWindow", -1, 2);
    SDL_Window *temp = window;
    if (temp != window || !temp)
      throw Exception("Failed to save old window to a temporary location", "replaceWindow", -1, 2);
    window = newWindow;
    if (window != newWindow || !window)
    {
      window = temp;
      if (window != temp || !window)
        throw Exception("Failed to overwrite old window with new one. Failed to return old window to previous state", "replaceWindow", -1, 2);
      throw Exception("Failed to overwrie old window with new one. Old window has been preserved", "replaceWindow", -1, 1);
    }
    SDL_DestroyWindow(temp);
    if (temp)
      throw Exception("Failed to destroy old window. This may cause memory leaks", "replaceWindow", -1, 1);
  }
  catch (Exception &err)
  {
    err << "Window";
    throw err;
  }
}

void Window::swapExecution() { execute = !execute; }

void Window::setExecution(const bool state) { execute = state; }

void Window::endExecution() { execute = false; }

void Window::changeTitle(const char *newTitle)
{
  SDL_SetWindowTitle(window, newTitle);
  if (SDL_GetWindowTitle(window) != newTitle)
    throw Exception("Failed to change window title", "Window::changeTitle", -1, 0);
}

void Window::changeResolution(const int width, const int height)
{
  int *oldWidth, *oldHeight;
  SDL_GetWindowSize(window, oldWidth, oldHeight);
  const int setWidth = width <= 0 ? *oldWidth : width;
  const int setHeight = height <= 0 ? *oldHeight : height;
  SDL_SetWindowSize(window, setWidth, setHeight);
}

void Window::updateFlags(uint32_t flags)
{
  uint32_t oldFlags = SDL_GetWindowFlags(window);
  if (flags & SDL_WINDOW_FULLSCREEN)
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (flags & SDL_WINDOW_SHOWN)
    SDL_ShowWindow(window);
  if (flags & SDL_WINDOW_HIDDEN)
    SDL_HideWindow(window);
  if (flags & SDL_WINDOW_BORDERLESS)
  {
    if (oldFlags & SDL_WINDOW_BORDERLESS)
      SDL_SetWindowBordered(window, SDL_TRUE);
    else
      SDL_SetWindowBordered(window, SDL_FALSE);
  }
  if (flags & SDL_WINDOW_RESIZABLE)
  {
    if (oldFlags & SDL_WINDOW_RESIZABLE)
      SDL_SetWindowResizable(window, SDL_FALSE);
    else
      SDL_SetWindowResizable(window, SDL_TRUE);
  }
}

// KeyboardEventHandler implementation
void KeyboardEventHandler::addKeyboardInput(const SDL_KeyCode key, const function<void(vector<void *> &)> &func, vector<void *> extradata, const bool state) // state 0 = Released, 1 = Pressed
{
  auto dup = keys.find(key);
  if (dup != keys.end())
    throw Exception("Tried to assign a function to a key that already has an "
                    "assigned function",
                    "Window::addKeyboardInput", -1, 1);
  keys.insert(make_pair(key, make_pair(func, make_pair(extradata, state))));
}

void KeyboardEventHandler::modifyKeyboardInput(const SDL_KeyCode key, const function<void(vector<void *> &)> &newfunc, vector<void *> extradata, const int state)
{
  auto it = keys.find(key);
  if (it == keys.end())
    throw Exception("Could not modify key " + to_string(key) +
                        ", key isn't bound",
                    "Window::modifyKeyboardInput", -1, 2);
  if (newfunc != NULL)
    it->second.first = newfunc;
  if (!extradata.empty())
    it->second.second.first = extradata;
  if (state == 0 || state == 1)
    it->second.second.second = state;
}

void KeyboardEventHandler::removeKeyboardInput(const SDL_KeyCode key) { keys.erase(key); }

void KeyboardEventHandler::cycleKeyboardInputs(SDL_Event *event)
{
  bool lctrl = false, rctrl = false, lalt = false, ralt = false, lshift = false,
       rshift = false;
  while (SDL_PollEvent(event))
  {
    if (event->key.keysym.sym == SDLK_LCTRL)
      lctrl = true;
    if (event->key.keysym.sym == SDLK_RCTRL)
      rctrl = true;
    if (event->key.keysym.sym == SDLK_LALT)
      lalt = true;
    if (event->key.keysym.sym == SDLK_RALT)
      ralt = true;
    if (event->key.keysym.sym == SDLK_LSHIFT)
      lshift = true;
    if (event->key.keysym.sym == SDLK_RSHIFT)
      rshift = true;
    if ((lalt || ralt) && event->key.keysym.sym == SDLK_F4) // Alt+F4
      exit(0);
    auto it = keys.find(static_cast<SDL_KeyCode>(event->key.keysym.sym));
    if (it != keys.end() && event->key.state == it->second.second.second)
      it->second.first(it->second.second.first);
  }
}

// GUIInteractables implementation
SDL_Rect *GUIInteractables::getRectangle(SDL_Rect *returnval)
{
  if (returnval != nullptr)
    returnval = rectangle;
  return rectangle;
}

GUIInteractables *GUIInteractables::getLeft(GUIInteractables *returnval)
{
  if (returnval != nullptr)
    returnval = left;
  return left;
}

GUIInteractables *GUIInteractables::getRight(GUIInteractables *returnval)
{
  if (returnval != nullptr)
    returnval = right;
  return right;
}

GUIInteractables *GUIInteractables::getUp(GUIInteractables *returnval)
{
  if (returnval != nullptr)
    returnval = up;
  return up;
}

GUIInteractables *GUIInteractables::getDown(GUIInteractables *returnval)
{
  if (returnval != nullptr)
    returnval = down;
  return down;
}

void GUIInteractables::addInteractable(GUIInteractables *newInteractable)
{
  if (newInteractable->getRectangle()->x < rectangle->x) // If on left side
  {
    if (left == nullptr)
      left = newInteractable;
    else
      left->addInteractable(newInteractable);
  }
  else if (newInteractable->getRectangle()->x +
               newInteractable->getRectangle()->w >
           rectangle->x + rectangle->w) // If on right side
  {
    if (right == nullptr)
      right = newInteractable;
    else
      right->addInteractable(newInteractable);
  }
  else if (newInteractable->getRectangle()->y < rectangle->y) // If above
  {
    if (up == nullptr)
      up = newInteractable;
    else
      up->addInteractable(newInteractable);
  }
  else if (newInteractable->getRectangle()->y +
               newInteractable->getRectangle()->h >
           rectangle->y + rectangle->h) // If below
  {
    if (down == nullptr)
      down = newInteractable;
    else
      down->addInteractable(newInteractable);
  }
  else
    throw Exception("Attempted to add GUI element that already exists or "
                    "overlaps another GUI element",
                    "GUIInteractables::addInteractable", -1, 2);
}

GUIInteractables *GUIInteractables::findItem(const int x, const int y, GUIInteractables *returnval)
{
  if (x < rectangle->x)
  {
    if (left == nullptr || !stackActive)
      return nullptr;
    else
      return findItem(x, y, returnval);
  }
  else if (x > rectangle->x + rectangle->w)
  {
    if (right == nullptr || !stackActive)
      return nullptr;
    else
      return findItem(x, y, returnval);
  }
  else if (y < rectangle->y)
  {
    if (up == nullptr || !stackActive)
      return nullptr;
    return findItem(x, y, returnval);
  }
  else if (y > rectangle->y + rectangle->h)
  {
    if (down == nullptr || !stackActive)
      return nullptr;
    else
      return findItem(x, y, returnval);
  }
  else
  {
    if (active)
      return this;
    else
      return nullptr;
  }
}

bool GUIInteractables::getActive(bool *returnval)
{
  if (returnval != nullptr)
    returnval = &active;
  return active;
}

void GUIInteractables::toggleActive() { active = !active; }

void GUIInteractables::setActive(const bool state) { active = state; }

void GUIInteractables::changeFunction(const function<void(vector<void *> &)> &newfunc)
{
  func = newfunc;
}

void GUIInteractables::changeButton(const int button)
{
  if (button == 0 || button == 1 || button == 2)
    mouseState = button;
}

bool GUIInteractables::getButton(bool *button)
{
  if (button != nullptr)
    button = &mouseState;
  return mouseState;
}

function<void(vector<void *> &)> GUIInteractables::getFunction(function<void(vector<void *> &)> returnval)
{
  if (returnval != nullptr)
    returnval = func;
  return func;
}

GUIInteractables::GUIInteractables(SDL_Rect *newRect, const function<void(vector<void *> &)> &newfunc, const bool button)
{
  rectangle = newRect;
  func = newfunc;
  mouseState = button;
}

// MouseEventHandler implementation
int MouseEventHandler::addGUI(GUIInteractables *firstElement)
{
  guis.insert(make_pair(guis.size() + 1, firstElement));
  return guis.size(); // GUI uuid
}

void MouseEventHandler::addGameInteractable(SDL_Rect *rectangle, const function<void(vector<void *> &)> &func, const bool mouseState, const int mouseButton)
{
  if (mouseState)
  {
    if (mouseButton == 0)
    {
      auto it = lHoldGameInt.find(rectangle);
      if (it != lHoldGameInt.end())
        throw Exception("Attempted to add a game interactable that already exists", "Window::addGameInteractable", -1, 2);
      lHoldGameInt.insert(make_pair(rectangle, func));
    }
    else if (mouseButton == 1)
    {
      auto it = mHoldGameInt.find(rectangle);
      if (it != mHoldGameInt.end())
        throw Exception("Attempted to add a game interactable that already exists", "Window::addGameInteractable", -1, 2);
      mHoldGameInt.insert(make_pair(rectangle, func));
    }
    else if (mouseButton == 2)
    {
      auto it = rHoldGameInt.find(rectangle);
      if (it != rHoldGameInt.end())
        throw Exception("Attempted to add a game interactable that already exists", "Window::addGameInteractable", -1, 2);
      rHoldGameInt.insert(make_pair(rectangle, func));
    }
    else
      throw Exception("Attempted to add a game interactable with a mouse button that doesn't exist", "Window::addGameInteractable", -1, 1);
  }
  else
  {
    if (mouseButton == 0)
    {
      auto it = lPushGameInt.find(rectangle);
      if (it != lPushGameInt.end())
        throw Exception("Attempted to add a game interactable that already exists", "Window::addGameInteractable", -1, 2);
      lPushGameInt.insert(make_pair(rectangle, func));
    }
    else if (mouseButton == 1)
    {
      auto it = mPushGameInt.find(rectangle);
      if (it != mPushGameInt.end())
        throw Exception("Attempted to add a game interactable that already exists", "Window::addGameInteractable", -1, 2);
      mPushGameInt.insert(make_pair(rectangle, func));
    }
    else if (mouseButton == 2)
    {
      auto it = rPushGameInt.find(rectangle);
      if (it != rPushGameInt.end())
        throw Exception("Attempted to add a game interactable that already exists", "Window::addGameInteractable", -1, 2);
      rPushGameInt.insert(make_pair(rectangle, func));
    }
    else
      throw Exception("Attempted to add a game interactable with a mouse button that doesn't exist", "Window::addGameInteractable", -1, 1);
  }
}
