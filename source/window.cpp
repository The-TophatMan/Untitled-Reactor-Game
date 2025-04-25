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
