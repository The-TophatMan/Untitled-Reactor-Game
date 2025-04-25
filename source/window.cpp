#include "../include/window.h"
#include "../include/exception.h"

// Window implementation
Window::Window(const char *title, const int width, const int height, uint32_t flags)
{
  exceptionStack.push_back(Exception("Creating new program window", "Window::Window", -1, INFO));
  if (width <= 0 || height <= 0)
  {
    exceptionStack.push_back(Exception("Provided width or height is too small", "Window::Window", -1, FATAL));
    return;
  }
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (!window)
  {
    exceptionStack.push_back(Exception("Failed to create window", "Window::Window", 1, FATAL));
    return;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
  {
    SDL_DestroyWindow(window);
    exceptionStack.push_back(Exception("Failed to create renderer", "Winow::Window", 1, FATAL));
    return;
  }
  exceptionStack.push_back(Exception("Created new program window", "Window::Window", -1, INFO));
}

Window::~Window()
{
  exceptionStack.push_back(Exception("Destroying program window", "Window::~Window", -1, INFO));
  SDL_DestroyRenderer(renderer);
  if (renderer)
    exceptionStack.push_back(Exception("Failed to destroy old renderer. This may cause memory leaks", "Window::~Window", -1, WARNING));
  SDL_DestroyWindow(window);
  if (window)
    exceptionStack.push_back(Exception("Failed to destroy window. This may cause memory leaks", "Window::~Window", -1, WARNING));
  exceptionStack.push_back(Exception("Finished destroying program window", "Window::~Window", -1, INFO));
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

const bool Window::isRunning(SDL_Event *event, bool *returnval)
{
  while (SDL_PollEvent(event))
    if (event->type == SDL_QUIT)
      execute = false;
  if (returnval != nullptr)
    *returnval = execute;
  return execute;
}

void Window::replaceRenderer(SDL_Renderer *newRenderer)
{
  exceptionStack.push_back(Exception("Replacing program window renderer", "Window::replaceRenderer", -1, INFO));
  if (!newRenderer)
  {
    exceptionStack.push_back(Exception("New Renderer did not properly initialize", "Window::replaceRenderer", -1, ERROR));
    return;
  }

  // Save old renderer
  exceptionStack.push_back(Exception("Saving old renderer to temporary location...", "Window::replaceRendereer", -1, INFO));
  SDL_Renderer *temp = renderer;
  if (temp != renderer || !temp)
    exceptionStack.push_back(Exception("Failed to save old renderer to a temporary location. If replacement fails, the renderer cannot be restored", "Window::replaceRenderer", -1, WARNING));
  else
    exceptionStack.push_back(Exception("Done!", "Window::replaceRenderer", -1, INFO));

  // Replace renderer
  exceptionStack.push_back(Exception("Replacing renderer...", "Window::replaceRenderer", -1, INFO));
  renderer = newRenderer;
  if (renderer != newRenderer || !renderer)
  {
    exceptionStack.push_back(Exception("Failed to overwrite old renderer with new one", "Window::replaceRenderer", -1, ERROR));
    exceptionStack.push_back(Exception("Restoring old renderer...", "Winddow::replaceRenderer", -1, INFO));
    renderer = temp;
    if (renderer != temp || !renderer)
    {
      exceptionStack.push_back(Exception("Failed to restore renderer to previous state", "replaceRenderer", -1, ERROR));
      return;
    }
    exceptionStack.push_back(Exception("Restored renderer to previous state", "Window::replaceRenderer", -1, INFO));
    return;
  }

  // Destroy old renderer
  exceptionStack.push_back(Exception("Destroying old renderer...", "Window::replaceRenderer", -1, INFO));
  SDL_DestroyRenderer(temp);
  if (temp)
    exceptionStack.push_back(Exception("Failed to destroy old renderer. This may cause memory leaks", "replaceRenderer", -1, WARNING));
  else
    exceptionStack.push_back(Exception("Done!", "Window::replaceRenderer", -1, INFO));
}

void Window::replaceWindow(SDL_Window *newWindow)
{
  exceptionStack.push_back(Exception("Replace program window window", "Window::replaceWindow", -1, INFO));
  if (!newWindow)
  {
    exceptionStack.push_back(Exception("New window did not properly initialize", "Window::replaceWindow", -1, ERROR));
    return;
  }

  // Save old window
  exceptionStack.push_back(Exception("Saving old window to temporary location...", "Window::replaceWindow", -1, INFO));
  SDL_Window *temp = window;
  if (temp != window || !temp)
    exceptionStack.push_back(Exception("Failed to save old window to a temporary location. If replacement fails, the old window will not be restored", "Window::replaceWindow", -1, WARNING));
  else
    exceptionStack.push_back(Exception("Done!", "Window::replaceWindow", -1, INFO));
  window = newWindow;
  if (window != newWindow || !window)
  {
    exceptionStack.push_back(Exception("Failed to overwrite old window with new one", "Window::replaceWindow", -1, ERROR));
    window = temp;
    if (window != temp || !window)
    {
      exceptionStack.push_back(Exception("Failed to return old window to previous state", "replaceWindow", -1, ERROR));
      return;
    }
    exceptionStack.push_back(Exception("Returned window to previous state", "Window::replaceWindow", -1, INFO));
    return;
  }
  SDL_DestroyWindow(temp);
  if (temp)
    exceptionStack.push_back(Exception("Failed to destroy old window. This may cause memory leaks", "Window::replaceWindow", -1, WARNING));
}

void Window::swapExecution() { execute = !execute; }

void Window::setExecution(const bool state) { execute = state; }

void Window::endExecution() { execute = false; }

void Window::changeTitle(const char *newTitle)
{
  SDL_SetWindowTitle(window, newTitle);
  if (SDL_GetWindowTitle(window) != newTitle)
    exceptionStack.push_back(Exception("Failed to change window title", "Window::changeTitle", -1, ERROR));
}

void Window::changeResolution(const int width, const int height)
{
  if (width <= 0 || height <= 0)
  {
    exceptionStack.push_back(Exception("Provided width or height is too small", "Window::changeResolution", -1, ERROR));
    return;
  }
  SDL_SetWindowSize(window, width, height);
}

void Window::updateFlags(uint32_t flags)
{
  exceptionStack.push_back(Exception("Updating window flags", "Window::updateFlags", -1, INFO));
  if (flags & SDL_WINDOW_FULLSCREEN)
  {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    exceptionStack.push_back(Exception("Changed fullscreen type", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
  {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    exceptionStack.push_back(Exception("Changed fullscreen desktop type", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_SHOWN)
  {
    SDL_ShowWindow(window);
    exceptionStack.push_back(Exception("Set window to be visible", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_HIDDEN)
  {
    SDL_HideWindow(window);
    exceptionStack.push_back(Exception("Set window to be invisible", "Window::updateFlags", -1, INFO));
  }
  if (flags & SDL_WINDOW_BORDERLESS)
  {
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS)
    {
      SDL_SetWindowBordered(window, SDL_TRUE);
      exceptionStack.push_back(Exception("Window has been toggled to be borderless", "Window::updateFlags", -1, INFO));
    }
    else
    {
      SDL_SetWindowBordered(window, SDL_FALSE);
      exceptionStack.push_back(Exception("Window has been toggled to be bordered", "Window::updateFlags", -1, INFO));
    }
  }
  if (flags & SDL_WINDOW_RESIZABLE)
  {
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_RESIZABLE)
    {
      SDL_SetWindowResizable(window, SDL_FALSE);
      exceptionStack.push_back(Exception("Window is no longer resizable", "Window::updateFlags", -1, INFO));
    }
    else
    {
      SDL_SetWindowResizable(window, SDL_TRUE);
      exceptionStack.push_back(Exception("Window is now resizable", "Window::updateFlags", -1, INFO));
    }
  }
}

vector<Exception> Window::errorStack(vector<Exception> *stack)
{
  if (stack != nullptr)
    *stack = exceptionStack;
  return exceptionStack;
}

void Window::flushErrorStack() { exceptionStack.clear(); }

// Keybind implementation
Keybind::Keybind(vector<SDL_KeyCode> binding,
                 function<void(vector<void *> &)> heldEvent,
                 function<void(vector<void *> &)> releasedEvent,
                 vector<void *> heldElements, vector<void *> releasedElements)
{
  exceptionStack.push_back(
      Exception("Creating new keybind class with multi-key",
                "Keybind::Keybind.0", -1, INFO));
  keybind = binding;
  exceptionStack.push_back(
      Exception("Set keybinding", "Keybind::Keybind.0", -1, INFO));
  held = heldEvent;
  exceptionStack.push_back(
      Exception("Set hold event", "Keybind::Keybind.0", -1, INFO));
  released = releasedEvent;
  exceptionStack.push_back(
      Exception("Set released event", "Keybind::Keybind.0", -1, INFO));
  heldInputs = heldElements;
  exceptionStack.push_back(Exception("Set hold event pass through elements",
                                     "Keybind::Keybind.0", -1, INFO));
  releasedInputs = releasedElements;
  exceptionStack.push_back(Exception("Set released event pass through elements",
                                     "Keybind::Keybind.0", -1, INFO));
  exceptionStack.push_back(Exception("Finished creating keybind with multi-key",
                                     "Keybind::Keybind.0", -1, INFO));
}

Keybind::Keybind(SDL_KeyCode key, function<void(vector<void *> &)> heldEvent,
                 vector<void *> heldElements,
                 function<void(vector<void *> &)> releasedEvent,
                 vector<void *> releasedElements)
{
  exceptionStack.push_back(
      Exception("Creating new keybind class with single key",
                "Keybind::Keybind.0", -1, INFO));
  keybind.push_back(key);
  exceptionStack.push_back(
      Exception("Set key", "Keybind::Keybind.0", -1, INFO));
  held = heldEvent;
  exceptionStack.push_back(
      Exception("Set hold event", "Keybind::Keybind.0", -1, INFO));
  released = releasedEvent;
  exceptionStack.push_back(
      Exception("Set released event", "Keybind::Keybind.0", -1, INFO));
  heldInputs = heldElements;
  exceptionStack.push_back(Exception("Set hold event pass through elements",
                                     "Keybind::Keybind.0", -1, INFO));
  releasedInputs = releasedElements;
  exceptionStack.push_back(Exception("Set released event pass through elements",
                                     "Keybind::Keybind.0", -1, INFO));
  exceptionStack.push_back(
      Exception("Finished creating keybind with single key",
                "Keybind::Keybind.0", -1, INFO));
}

const std::function<void(vector<void *> &)> Keybind::getHoldEvent(function<void(vector<void *> &)> *returnval)
{
  if (returnval != nullptr)
    returnval = &held;
  return held;
}

const std::function<void(vector<void *> &)> Keybind::getReleasedEvent(function<void(vector<void *> &)> *returnval)
{
  if (returnval != nullptr)
    returnval = &released;
  return released;
}

const std::vector<void *> Keybind::getHeldElements(vector<void *> *returnval)
{
  if (returnval != nullptr)
    returnval = &heldInputs;
  return heldInputs;
}

const std::vector<void *> Keybind::getReleasedElements(vector<void *> *returnval)
{
  if (returnval != nullptr)
    returnval = &releasedInputs;
  return releasedInputs;
}

const bool Keybind::isActive(bool *returnval)
{
  if (returnval != nullptr)
    returnval = &active;
  return active;
}

const std::vector<SDL_KeyCode> Keybind::getKeybind(vector<SDL_KeyCode> *returnval)
{
  if (returnval != nullptr)
    returnval = &keybind;
  return keybind;
}

void Keybind::replaceHoldEvent(function<void(vector<void *> &)> newEvent,
                               vector<void *> holdElements)
{
  exceptionStack.push_back(
      Exception("Replacing hold event", "Keybind::replaceHoldEvent", -1, INFO));
  // Save old event
  exceptionStack.push_back(
      Exception("Saving old event to temporary location...",
                "Keybind::replaceHoldEvent", -1, INFO));
  const function<void(vector<void *> &)> temp = held;
  if (&temp != &held)
    exceptionStack.push_back(
        Exception("Failed to save old event to temporary location. If "
                  "replacement fails, the old function won't be restored",
                  "Keybind::replaceHoldEvent", -1, WARNING));
  else
    exceptionStack.push_back(
        Exception("Done!", "Keybind::replaceHoldEvent", -1, INFO));

  // Save old elements
  exceptionStack.push_back(
      Exception("Saving old elements to temporary location...",
                "Keybind::replaceHoldEvent", -1, INFO));
  vector<void *> temp2 = heldInputs;
  if (temp2 != heldInputs)
    exceptionStack.push_back(
        Exception("Failed to save old elements. If replacement fails, the old "
                  "elements won't be restored",
                  "Keybind::replaceHoldEvent", -1, ERROR));
  else
    exceptionStack.push_back(
        Exception("Done!", "Keybind::replaceHoldEvent", -1, INFO));

  // Replace event
  exceptionStack.push_back(Exception("Attempting to replace event...",
                                     "Keybind::replaceHoldEvent", -1, INFO));
  held = newEvent;
  if (&held != &newEvent)
  {
    exceptionStack.push_back(Exception("Failed to replace event",
                                       "Keybind::replaceHoldEvent", -1, ERROR));
    exceptionStack.push_back(Exception("Attempting to revert to old event...",
                                       "Keybind::replaceHoldEvent", -1, INFO));
    held = temp;
    if (&held != &temp)
      exceptionStack.push_back(Exception("Failed to revert to old event",
                                         "Keybind::replaceHoldEvent", -1,
                                         ERROR));
    else
      exceptionStack.push_back(Exception(
          "Reverted to old event", "Keybind::replaceHoldEvent", -1, INFO));
  }
  else
    exceptionStack.push_back(Exception("Successfully replaced hold event",
                                       "Keybind::replaceHoldEvent", -1, INFO));
}

void Keybind::replaceReleasedEvent(function<void(vector<void *> &)> newEvent,
                                   vector<void *> releasedElements)
{
  exceptionStack.push_back(Exception(
      "Replacing released event", "Keybind::replaceReleasedEvent", -1, INFO));
  // Save old event
  exceptionStack.push_back(
      Exception("Saving old event to temporary location...",
                "Keybind::replaceReleasedEvent", -1, INFO));
  const function<void(vector<void *> &)> temp = released;
  if (&temp != &released)
    exceptionStack.push_back(
        Exception("Failed to save old event to temporary location. If "
                  "replacement fails, the old function won't be restored",
                  "Keybind::replaceReleasedEvent", -1, WARNING));
  else
    exceptionStack.push_back(
        Exception("Done!", "Keybind::replaceReleasedEvent", -1, INFO));

  // Save old elements
  exceptionStack.push_back(
      Exception("Saving old elements to temporary location...",
                "Keybind::replaceReleasedEvent", -1, INFO));
  vector<void *> temp2 = releasedInputs;
  if (temp2 != releasedInputs)
    exceptionStack.push_back(
        Exception("Failed to save old elements. If replacement fails, the old "
                  "elements won't be restored",
                  "Keybind::replaceReleasedEvent", -1, ERROR));
  else
    exceptionStack.push_back(
        Exception("Done!", "Keybind::replaceReleasedEvent", -1, INFO));

  // Replace event
  exceptionStack.push_back(Exception("Attempting to replace event...",
                                     "Keybind::replaceReleasedEvent", -1,
                                     INFO));
  released = newEvent;
  if (&released != &newEvent)
  {
    exceptionStack.push_back(Exception(
        "Failed to replace event", "Keybind::replaceReleasedEvent", -1, ERROR));
    exceptionStack.push_back(Exception("Attempting to revert to old event...",
                                       "Keybind::replaceReleasedEvent", -1,
                                       INFO));
    released = temp;
    if (&released != &temp)
      exceptionStack.push_back(Exception("Failed to revert to old event",
                                         "Keybind::replaceReleasedEvent", -1,
                                         ERROR));
    else
      exceptionStack.push_back(Exception(
          "Reverted to old event", "Keybind::replaceReleasedEvent", -1, INFO));
  }
  else
    exceptionStack.push_back(Exception("Successfully replaced released event",
                                       "Keybind::replaceReleasedEvent", -1,
                                       INFO));
}

void Keybind::toggleActive() { active = !active; }

void Keybind::setActive(const bool set) { active = set; }

void Keybind::deactivate() { active = false; }

void Keybind::activate() { active = true; }

void Keybind::removeHoldEvent()
{
  exceptionStack.push_back(
      Exception("Removing hold event", "Keybind::removeHoldEvent", -1, INFO));
  held = nullptr;
  heldInputs.clear();
}

void Keybind::removeReleasedEvent()
{
  exceptionStack.push_back(Exception("Removing released event",
                                     "Keybind::removeReleasedEvent", -1, INFO));
  released = nullptr;
  releasedInputs.clear();
}

std::vector<Exception> Keybind::errorStack(vector<Exception> *retrunval)
{
  if (retrunval != nullptr)
    retrunval = &exceptionStack;
  return exceptionStack;
}

void Keybind::flushErrorStack() { exceptionStack.clear(); }

// Keyboard Event Handler implementation

// Mouse Interactable implementation

SDL_Rect *MouseInteractable::getCollisionBox(SDL_Rect *returnval)
{
  if (returnval != nullptr)
    returnval = collision;
  return collision;
}

function<void(vector<void *> &)> MouseInteractable::getLeftClickFunction(function<void(vector<void *> &)> returnval)
{
  if (returnval != nullptr)
    returnval = lcFunc;
  return lcFunc;
}

function<void(vector<void *> &)> MouseInteractable::getRightClickFunction(function<void(vector<void *> &)> returnval)
{
  if (returnval != nullptr)
    returnval = rcFunc;
  return rcFunc;
}

function<void(vector<void *> &)> MouseInteractable::getMiddleClickFunction(function<void(vector<void *> &)> returnval)
{
  if (returnval != nullptr)
    returnval = mcFunc;
  return mcFunc;
}

function<void(vector<void *> &)> MouseInteractable::getLeftHoldFunction(function<void(vector<void *> &)> returnval)
{
  if (returnval != nullptr)
    returnval = lhFunc;
  return lhFunc;
}

function<void(vector<void *> &)> MouseInteractable::getRightHoldFunction(function<void(vector<void *> &)> returnval)
{
  if (returnval != nullptr)
    returnval = rhFunc;
  return rhFunc;
}

function<void(vector<void *> &)> MouseInteractable::getMiddleHoldFunction(function<void(vector<void *> &)> returnval)
{
  if (returnval != nullptr)
    returnval = mhFunc;
  return mhFunc;
}

std::vector<Exception> MouseInteractable::errorStack(vector<Exception> *errorStack)
{
  if (errorStack != nullptr)
    *errorStack = exceptionStack;
  return exceptionStack;
}

void MouseInteractable::flushErrorStack() { exceptionStack.clear(); }
