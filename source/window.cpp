#include "../include/window.h"
#include "../include/exception.h"
#include <SDL2/SDL_events.h>

// Window implementation
Window::Window(const char *title, const int width, const int height, uint32_t flags)
{
  stack.insert(Exception("Creating new program window", "Window::Window", -1, INFO));
  if (width <= 0 || height <= 0)
  {
    stack.insert(Exception("Provided width or height is too small", "Window::Window", -1, FATAL));
    return;
  }
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (!window)
  {
    stack.insert(Exception("Failed to create window", "Window::Window", 1, FATAL));
    return;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    SDL_DestroyWindow(window);
    stack.insert(Exception("Failed to create renderer", "Winow::Window", 1, FATAL));
    return;
  }
  stack.insert(Exception("Created new program window", "Window::Window", -1, INFO));
}

Window::~Window()
{
  stack.insert(Exception("Destroying program window", "Window::~Window", -1, INFO));
  SDL_DestroyRenderer(renderer);
  if (renderer)
    stack.insert(Exception("Failed to destroy old renderer. This may cause memory leaks", "Window::~Window", -1, WARNING));
  SDL_DestroyWindow(window);
  if (window)
    stack.insert(Exception("Failed to destroy window. This may cause memory leaks", "Window::~Window", -1, WARNING));
  stack.insert(Exception("Finished destroying program window", "Window::~Window", -1, INFO));
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

bool Window::isRunning(SDL_Event *event, bool *returnval)
{
  while (SDL_PollEvent(event))
    if (event->type == SDL_QUIT)
    {
      execute = false;
      break;
    }
  if (returnval != nullptr)
    *returnval = execute;
  return execute;
}

void Window::replaceRenderer(SDL_Renderer *newRenderer)
{
  stack.insert(Exception("Replacing program window renderer", "Window::replaceRenderer", -1, INFO));
  if (!newRenderer)
  {
    stack.insert(Exception("New Renderer did not properly initialize", "Window::replaceRenderer", -1, ERROR));
    return;
  }

  // Save old renderer
  stack.insert(Exception("Saving old renderer to temporary location...", "Window::replaceRendereer", -1, INFO));
  SDL_Renderer *temp = renderer;
  if (temp != renderer || !temp)
    stack.insert(Exception("Failed to save old renderer to a temporary location. If replacement fails, the renderer cannot be restored", "Window::replaceRenderer", -1, WARNING));
  else
    stack.insert(Exception("Done!", "Window::replaceRenderer", -1, INFO));

  // Replace renderer
  stack.insert(Exception("Replacing renderer...", "Window::replaceRenderer", -1, INFO));
  renderer = newRenderer;
  if (renderer != newRenderer || !renderer)
  {
    stack.insert(Exception("Failed to overwrite old renderer with new one", "Window::replaceRenderer", -1, ERROR));
    stack.insert(Exception("Restoring old renderer...", "Winddow::replaceRenderer", -1, INFO));
    renderer = temp;
    if (renderer != temp || !renderer)
    {
      stack.insert(Exception("Failed to restore renderer to previous state", "replaceRenderer", -1, ERROR));
      return;
    }
    stack.insert(Exception("Restored renderer to previous state", "Window::replaceRenderer", -1, INFO));
    return;
  }

  // Destroy old renderer
  stack.insert(Exception("Destroying old renderer...", "Window::replaceRenderer", -1, INFO));
  SDL_DestroyRenderer(temp);
  if (temp)
    stack.insert(Exception("Failed to destroy old renderer. This may cause memory leaks", "replaceRenderer", -1, WARNING));
  else
    stack.insert(Exception("Done!", "Window::replaceRenderer", -1, INFO));
}

void Window::replaceWindow(SDL_Window *newWindow)
{
  stack.insert(Exception("Replace program window window", "Window::replaceWindow", -1, INFO));
  if (!newWindow)
  {
    stack.insert(Exception("New window did not properly initialize", "Window::replaceWindow", -1, ERROR));
    return;
  }

  // Save old window
  stack.insert(Exception("Saving old window to temporary location...", "Window::replaceWindow", -1, INFO));
  SDL_Window *temp = window;
  if (temp != window || !temp)
    stack.insert(Exception("Failed to save old window to a temporary location. If replacement fails, the old window will not be restored", "Window::replaceWindow", -1, WARNING));
  else
    stack.insert(Exception("Done!", "Window::replaceWindow", -1, INFO));
  window = newWindow;
  if (window != newWindow || !window)
  {
    stack.insert(Exception("Failed to overwrite old window with new one", "Window::replaceWindow", -1, ERROR));
    window = temp;
    if (window != temp || !window)
    {
      stack.insert(Exception("Failed to return old window to previous state", "replaceWindow", -1, ERROR));
      return;
    }
    stack.insert(Exception("Returned window to previous state", "Window::replaceWindow", -1, INFO));
    return;
  }
  SDL_DestroyWindow(temp);
  if (temp)
    stack.insert(Exception("Failed to destroy old window. This may cause memory leaks", "Window::replaceWindow", -1, WARNING));
}

void Window::swapExecution() { execute = !execute; }

void Window::setExecution(const bool state) { execute = state; }

void Window::endExecution() { execute = false; }

void Window::changeTitle(const char *newTitle)
{
  SDL_SetWindowTitle(window, newTitle);
  if (SDL_GetWindowTitle(window) != newTitle)
    stack.insert(Exception("Failed to change window title", "Window::changeTitle", -1, ERROR));
}

void Window::changeResolution(const int width, const int height)
{
  if (width <= 0 || height <= 0)
  {
    stack.insert(Exception("Provided width or height is too small", "Window::changeResolution", -1, ERROR));
    return;
  }
  SDL_SetWindowSize(window, width, height);
}

void Window::updateFlags(uint32_t flags)
{
  stack.insert(Exception("Updating window flags", "Window::updateFlags", -1, INFO));
  if (flags & SDL_WINDOW_FULLSCREEN)
  {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    stack.insert(Exception("Changed fullscreen type", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
  {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    stack.insert(Exception("Changed fullscreen desktop type", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_SHOWN)
  {
    SDL_ShowWindow(window);
    stack.insert(Exception("Set window to be visible", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_HIDDEN)
  {
    SDL_HideWindow(window);
    stack.insert(Exception("Set window to be invisible", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_BORDERLESS)
  {
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS)
    {
      SDL_SetWindowBordered(window, SDL_TRUE);
      stack.insert(Exception("Window has been toggled to be borderless", "Window::updateFlags", -1, INFO));
    }
    else
    {
      SDL_SetWindowBordered(window, SDL_FALSE);
      stack.insert(Exception("Window has been toggled to be bordered", "Window::updateFlags", -1, INFO));
    }
  }
  if (flags & SDL_WINDOW_RESIZABLE)
  {
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_RESIZABLE)
    {
      SDL_SetWindowResizable(window, SDL_FALSE);
      stack.insert(Exception("Window is no longer resizable", "Window::updateFlags", -1, INFO));
    }
    else
    {
      SDL_SetWindowResizable(window, SDL_TRUE);
      stack.insert(Exception("Window is now resizable", "Window::updateFlags", -1, INFO));
    }
  }
}

ExceptionStack *Window::errorStack(ExceptionStack *returnval)
{
  if (returnval != nullptr)
    returnval = &stack;
  return &stack;
}

// Keybind implementation
Keybind::Keybind(SDL_KeyCode key,
                 Uint16 modifier,
                 function<void(vector<string> &)> heldEvent,
                 vector<string> heldElements,
                 function<void(vector<string> &)> releasedEvent,
                 vector<string> releasedElements)
{
  stack.insert(Exception("Creating new keybind", "Keybind::Keybind.0", -1, INFO));
  keybind = key;
  stack.insert(Exception("Set key", "Keybind::Keybind", -1, INFO));
  mod = modifier;
  stack.insert(Exception("Set modifier", "Keybind::Keybind", -1, INFO));
  held = heldEvent;
  stack.insert(Exception("Set hold event", "Keybind::Keybind", -1, INFO));
  released = releasedEvent;
  stack.insert(Exception("Set released event", "Keybind::Keybind", -1, INFO));
  heldInputs = heldElements;
  stack.insert(Exception("Set hold event pass through elements", "Keybind::Keybind", -1, INFO));
  releasedInputs = releasedElements;
  stack.insert(Exception("Set released event pass through elements", "Keybind::Keybind", -1, INFO));
  stack.insert(Exception("Finished creating keybind", "Keybind::Keybind", -1, INFO));
}

function<void(vector<string> &)> Keybind::getHoldEvent(function<void(vector<string> &)> *returnval)
{
  if (returnval != nullptr)
    returnval = &held;
  return held;
}

function<void(vector<string> &)> Keybind::getReleasedEvent(function<void(vector<string> &)> *returnval)
{
  if (returnval != nullptr)
    returnval = &released;
  return released;
}

vector<string> Keybind::getHeldElements(vector<string> *returnval)
{
  if (returnval != nullptr)
    returnval = &heldInputs;
  return heldInputs;
}

vector<string> Keybind::getReleasedElements(vector<string> *returnval)
{
  if (returnval != nullptr)
    returnval = &releasedInputs;
  return releasedInputs;
}

bool Keybind::isActive(bool *returnval)
{
  if (returnval != nullptr)
    returnval = &active;
  return active;
}

SDL_KeyCode Keybind::getKeybind(SDL_KeyCode *returnval)
{
  if (returnval != nullptr)
    returnval = &keybind;
  return keybind;
}

Uint16 Keybind::getModifier(Uint16 *returnval)
{
  if (returnval != nullptr)
    returnval = &mod;
  return mod;
}

void Keybind::replaceHoldEvent(function<void(vector<string> &)> newEvent, vector<string> holdElements)
{
  stack.insert(Exception("Replacing hold event", "Keybind::replaceHoldEvent", -1, INFO));
  // Save old event
  stack.insert(Exception("Saving old event to temporary location...", "Keybind::replaceHoldEvent", -1, INFO));
  const function<void(vector<string> &)> temp = held;
  if (&temp != &held)
    stack.insert(Exception("Failed to save old event to temporary location. If replacement fails, the old function won't be restored", "Keybind::replaceHoldEvent", -1, WARNING));
  else
    stack.insert(Exception("Done!", "Keybind::replaceHoldEvent", -1, INFO));

  // Save old elements
  stack.insert(Exception("Saving old elements to temporary location...", "Keybind::replaceHoldEvent", -1, INFO));
  vector<string> temp2 = heldInputs;
  if (temp2 != heldInputs)
    stack.insert(Exception("Failed to save old elements. If replacement fails, the old elements won't be restored", "Keybind::replaceHoldEvent", -1, ERROR));
  else
    stack.insert(Exception("Done!", "Keybind::replaceHoldEvent", -1, INFO));

  // Replace event
  stack.insert(Exception("Attempting to replace event...", "Keybind::replaceHoldEvent", -1, INFO));
  held = newEvent;
  if (&held != &newEvent)
  {
    stack.insert(Exception("Failed to replace event", "Keybind::replaceHoldEvent", -1, ERROR));
    stack.insert(Exception("Attempting to revert to old event...", "Keybind::replaceHoldEvent", -1, INFO));
    held = temp;
    if (&held != &temp)
      stack.insert(Exception("Failed to revert to old event",
                             "Keybind::replaceHoldEvent", -1,
                             ERROR));
    else
      stack.insert(Exception(
          "Reverted to old event", "Keybind::replaceHoldEvent", -1, INFO));
  }
  else
    stack.insert(Exception("Successfully replaced hold event",
                           "Keybind::replaceHoldEvent", -1, INFO));
}

void Keybind::replaceReleasedEvent(function<void(vector<string> &)> newEvent, vector<string> releasedElements)
{
  stack.insert(Exception(
      "Replacing released event", "Keybind::replaceReleasedEvent", -1, INFO));
  // Save old event
  stack.insert(
      Exception("Saving old event to temporary location...",
                "Keybind::replaceReleasedEvent", -1, INFO));
  const function<void(vector<string> &)> temp = released;
  if (&temp != &released)
    stack.insert(
        Exception("Failed to save old event to temporary location. If "
                  "replacement fails, the old function won't be restored",
                  "Keybind::replaceReleasedEvent", -1, WARNING));
  else
    stack.insert(
        Exception("Done!", "Keybind::replaceReleasedEvent", -1, INFO));

  // Save old elements
  stack.insert(
      Exception("Saving old elements to temporary location...",
                "Keybind::replaceReleasedEvent", -1, INFO));
  vector<string> temp2 = releasedInputs;
  if (temp2 != releasedInputs)
    stack.insert(
        Exception("Failed to save old elements. If replacement fails, the old "
                  "elements won't be restored",
                  "Keybind::replaceReleasedEvent", -1, ERROR));
  else
    stack.insert(
        Exception("Done!", "Keybind::replaceReleasedEvent", -1, INFO));

  // Replace event
  stack.insert(Exception("Attempting to replace event...",
                         "Keybind::replaceReleasedEvent", -1,
                         INFO));
  released = newEvent;
  if (&released != &newEvent)
  {
    stack.insert(Exception(
        "Failed to replace event", "Keybind::replaceReleasedEvent", -1, ERROR));
    stack.insert(Exception("Attempting to revert to old event...",
                           "Keybind::replaceReleasedEvent", -1,
                           INFO));
    released = temp;
    if (&released != &temp)
      stack.insert(Exception("Failed to revert to old event",
                             "Keybind::replaceReleasedEvent", -1,
                             ERROR));
    else
      stack.insert(Exception(
          "Reverted to old event", "Keybind::replaceReleasedEvent", -1, INFO));
  }
  else
    stack.insert(Exception("Successfully replaced released event",
                           "Keybind::replaceReleasedEvent", -1,
                           INFO));
}

void Keybind::replaceModifier(const Uint16 newModifier)
{
  stack.insert(Exception("Replacing key modifier",
                         "Keybind::replaceUint16", -1, INFO));
  stack.insert(
      Exception("Saving old modifier to temporary location...",
                "Keybind::replaceUint16", -1, INFO));
  Uint16 temp = mod;
  if (temp != mod)
    stack.insert(
        Exception("Failed to save old modifier to temporary location. If "
                  "replacement fails, the old modifier won't be restored",
                  "Keybind::replaceUint16", -1, WARNING));
  else
    stack.insert(
        Exception("Done!", "Keybind::replaceUint16", -1, INFO));
  stack.insert(Exception("Attempting to replace modifier...",
                         "Keybind::replaceUint16", -1, ERROR));
  mod = newModifier;
  if (mod != newModifier)
  {
    stack.insert(Exception("Failed to replace modifier",
                           "Keybind::replaceUint16", -1,
                           ERROR));
    stack.insert(Exception("Attempting to restore old modifier...",
                           "Keybind::replaceUint16", -1,
                           INFO));
    mod = temp;
    if (mod != temp)
      stack.insert(Exception("Failed to restore modifier",
                             "Keybind::replaceUint16", -1,
                             ERROR));
    else
      stack.insert(Exception(
          "Old modifier restored", "Keybind::replaceUint16", -1, INFO));
  }
  else
    stack.insert(Exception("Successfully replaced modifier",
                           "Keybind::replaceUint16", -1,
                           INFO));
}

void Keybind::toggleActive() { active = !active; }

void Keybind::setActive(const bool set) { active = set; }

void Keybind::deactivate() { active = false; }

void Keybind::activate() { active = true; }

void Keybind::removeHoldEvent()
{
  stack.insert(Exception("Removing hold event", "Keybind::removeHoldEvent", -1, INFO));
  held = nullptr;
  heldInputs.clear();
}

void Keybind::removeReleasedEvent()
{
  stack.insert(Exception("Removing released event",
                         "Keybind::removeReleasedEvent", -1, INFO));
  released = nullptr;
  releasedInputs.clear();
}

void Keybind::removeModifier()
{
  stack.insert(Exception("Removing key modifier",
                         "Keybind::removeUint16", -1, INFO));
  mod = 0;
}

ExceptionStack *Keybind::errorStack(ExceptionStack *retrunval)
{
  if (retrunval != nullptr)
    retrunval = &stack;
  return &stack;
}

// Keyboard Event Handler implementation
KeyboardEventHandler::KeyboardEventHandler(map<pair<SDL_KeyCode, Uint16>, Keybind *> import)
{
  stack.insert(Exception("Creating new Keyboard Event Handler",
                         "KeyboardEventHandler::KeyboardEventHandler", -1,
                         INFO));
  if (!import.empty())
    keybinds = import;
}

void KeyboardEventHandler::addKeybind(Keybind *keybind)
{
  stack.insert(Exception("Adding keybind to handler", "KeyboardEventHandler::addKeybind", -1, INFO));
  keybinds.insert(make_pair(make_pair(keybind->getKeybind(), keybind->getModifier()), keybind));
}

Keybind *KeyboardEventHandler::getKeybind(SDL_KeyCode key, Uint16 modifier, Keybind *returnval)
{
  Keybind *val = keybinds.find(make_pair(key, modifier))->second;
  if (returnval != nullptr)
    returnval = val;
  return val;
}

Keybind *KeyboardEventHandler::getKeybind(Keybind *bind, Keybind *returnval)
{
  Keybind *val =
      keybinds.find(make_pair(bind->getKeybind(), bind->getModifier()))->second;
  if (returnval != nullptr)
    returnval = val;
  return val;
}

void KeyboardEventHandler::removeKeybind(SDL_KeyCode key, Uint16 modifier)
{
  stack.insert(Exception("Removing keybind from handler",
                         "KeyboardEventHandler::removeKeybind.0", -1, INFO));
  keybinds.erase(make_pair(key, modifier));
}

void KeyboardEventHandler::removeKeybind(Keybind *bind)
{
  stack.insert(Exception("Removing keybind from handler",
                         "KeyboardEventHandler::removeKeybind.1", -1, INFO));
  keybinds.erase(make_pair(bind->getKeybind(), bind->getModifier()));
}

ExceptionStack *KeyboardEventHandler::errorStack(ExceptionStack *retrunval)
{
  if (retrunval != nullptr)
    retrunval = &stack;
  return &stack;
}

// Mouse Interactable implementation

SDL_Rect *MouseInteractable::getCollisionBox(SDL_Rect *returnval)
{
  if (returnval != nullptr)
    returnval = collision;
  return collision;
}

function<void(vector<string> &)> MouseInteractable::getLeftClickFunction(function<void(vector<string> &)> returnval)
{
  if (returnval != nullptr)
    returnval = lcFunc;
  return lcFunc;
}

function<void(vector<string> &)> MouseInteractable::getRightClickFunction(function<void(vector<string> &)> returnval)
{
  if (returnval != nullptr)
    returnval = rcFunc;
  return rcFunc;
}

function<void(vector<string> &)> MouseInteractable::getMiddleClickFunction(function<void(vector<string> &)> returnval)
{
  if (returnval != nullptr)
    returnval = mcFunc;
  return mcFunc;
}

function<void(vector<string> &)> MouseInteractable::getLeftHoldFunction(function<void(vector<string> &)> returnval)
{
  if (returnval != nullptr)
    returnval = lhFunc;
  return lhFunc;
}

function<void(vector<string> &)> MouseInteractable::getRightHoldFunction(function<void(vector<string> &)> returnval)
{
  if (returnval != nullptr)
    returnval = rhFunc;
  return rhFunc;
}

function<void(vector<string> &)> MouseInteractable::getMiddleHoldFunction(function<void(vector<string> &)> returnval)
{
  if (returnval != nullptr)
    returnval = mhFunc;
  return mhFunc;
}

ExceptionStack *MouseInteractable::errorStack(ExceptionStack *errorStack)
{
  if (errorStack != nullptr)
    errorStack = &stack;
  return &stack;
}
