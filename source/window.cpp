#include "../include/window.h"
#include "../include/exception.h"

// Window implementation
Window::Window(const char* title, const int width, const int height, uint32_t flags)
{
  if (width <= 0 || height <= 0) {
    exceptionStack.push_back(Exception("Provided width or height is too small", "Window::Window", -1, FATAL));
    return;
  }
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (!window) {
    exceptionStack.push_back(Exception("Failed to create window", "Window::Window", 1, FATAL));
    return;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_DestroyWindow(window);
    exceptionStack.push_back(Exception("Failed to create renderer", "Winow::Window", 1, FATAL));
    return;
  }
}

Window::~Window()
{
  SDL_DestroyRenderer(renderer);
  if (renderer)
    exceptionStack.push_back(Exception("Failed to destroy old renderer. This may cause memory leaks", "Window::~Window", -1, WARNING));
  SDL_DestroyWindow(window);
  if (window)
    exceptionStack.push_back(Exception("Failed to destroy window. This may cause memory leaks", "Window::~Window", -1, WARNING));
}

SDL_Renderer* Window::getRenderer(SDL_Renderer* returnval)
{
  if (returnval != nullptr)
    returnval = renderer;
  return renderer;
}

SDL_Window* Window::getWindow(SDL_Window* returnval)
{
  if (returnval != nullptr)
    returnval = window;
  return window;
}

const bool Window::isRunning(SDL_Event* event, bool* returnval)
{
  while (SDL_PollEvent(event))
    if (event->type == SDL_QUIT)
      execute = false;
  if (returnval != nullptr)
    *returnval = execute;
  return execute;
}

void Window::replaceRenderer(SDL_Renderer* newRenderer)
{
  if (!newRenderer) {
    exceptionStack.push_back(Exception("New Renderer did not properly initialize", "Window::replaceRenderer", -1, ERROR));
    return;
  }
  SDL_Renderer* temp = renderer;
  if (temp != renderer || !temp)
    exceptionStack.push_back(Exception("Failed to save old renderer to a temporary location. If replacement fails, the renderer cannot be restored", "Window::replaceRenderer", -1, WARNING));
  renderer = newRenderer;
  if (renderer != newRenderer || !renderer)
  {
    exceptionStack.push_back(Exception("Failed to overwrite old renderer with new one.", "Window::replaceRenderer", -1, ERROR));
    renderer = temp;
    if (renderer != temp || !renderer) {
      exceptionStack.push_back(Exception("Failed to return renderer to previous state", "replaceRenderer", -1, ERROR));
      return;
    }
    exceptionStack.push_back(Exception("Returned renderer to previous state", "Window::replaceRenderer", -1, INFO));
    return;
  }
  SDL_DestroyRenderer(temp);
  if (temp)
    exceptionStack.push_back(Exception("Failed to destroy old renderer. This may cause memory leaks", "replaceRenderer", -1, WARNING));
}

void Window::replaceWindow(SDL_Window* newWindow)
{
  if (!newWindow) {
    exceptionStack.push_back(Exception("New window did not properly initialize", "Window::replaceWindow", -1, ERROR));
    return;
  }
  SDL_Window* temp = window;
  if (temp != window || !temp)
    exceptionStack.push_back(Exception("Failed to save old window to a temporary location. If replacement fails, the old window will not be restored", "Window::replaceWindow", -1, WARNING));
  window = newWindow;
  if (window != newWindow || !window)
  {
    exceptionStack.push_back(Exception("Failed to overwrite old window with new one", "Window::replaceWindow", -1, ERROR));
    window = temp;
    if (window != temp || !window) {
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

void Window::changeTitle(const char* newTitle)
{
  SDL_SetWindowTitle(window, newTitle);
  if (SDL_GetWindowTitle(window) != newTitle)
    exceptionStack.push_back(Exception("Failed to change window title", "Window::changeTitle", -1, ERROR));
}

void Window::changeResolution(const int width, const int height)
{
  if (width <= 0 || height <= 0) {
    exceptionStack.push_back(Exception("Provided width or height is too small", "Window::changeResolution", -1, ERROR));
    return;
  }
  SDL_SetWindowSize(window, width, height);
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

vector<Exception> Window::errorStack(vector<Exception>* stack) {
  if (stack != nullptr)
    *stack = exceptionStack;
  return exceptionStack;
}

void Window::flushErrorStack() { exceptionStack.clear(); }

// Keybind implementation

// Keyboard Event Handler implementation

// Mouse Interactable implementation

MouseInteractable::MouseInteractable(
  SDL_Rect* collisionbox, const function<void(vector<void*>&)> leftclick,
  const function<void(vector<void*>&)> rightclick,
  const function<void(vector<void*>&)> middleclick,
  const function<void(vector<void*>&)> lefthold,
  const function<void(vector<void*>&)> righthold,
  const function<void(vector<void*>&)> middlehold) {
  lhFunc = lefthold;
  rhFunc = righthold;
  mhFunc = middlehold;
  lcFunc = leftclick;
  rcFunc = rightclick;
  mcFunc = middleclick;
  if (!collisionbox) {
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

SDL_Rect* MouseInteractable::getCollisionBox(SDL_Rect* returnval) {
  if (returnval != nullptr)
    returnval = collision;
  return collision;
}

function<void(vector<void*>&)> MouseInteractable::getLeftClickFunction(function<void(vector<void*>&)> returnval) {
  if (returnval != nullptr)
    returnval = lcFunc;
  return lcFunc;
}

function<void(vector<void*>&)> MouseInteractable::getRightClickFunction(function<void(vector<void*>&)> returnval) {
  if (returnval != nullptr)
    returnval = rcFunc;
  return rcFunc;
}

function<void(vector<void*>&)> MouseInteractable::getMiddleClickFunction(function<void(vector<void*>&)> returnval) {
  if (returnval != nullptr)
    returnval = mcFunc;
  return mcFunc;
}

function<void(vector<void*>&)> MouseInteractable::getLeftHoldFunction(function<void(vector<void*>&)> returnval) {
  if (returnval != nullptr)
    returnval = lhFunc;
  return lhFunc;
}

function<void(vector<void*>&)> MouseInteractable::getRightHoldFunction(function<void(vector<void*>&)> returnval) {
  if (returnval != nullptr)
    returnval = rhFunc;
  return rhFunc;
}

function<void(vector<void*>&)> MouseInteractable::getMiddleHoldFunction(function<void(vector<void*>&)> returnval) {
  if (returnval != nullptr)
    returnval = mhFunc;
  return mhFunc;
}

std::vector<Exception> MouseInteractable::errorStack(vector<Exception>* errorStack) {
  if (errorStack != nullptr)
    *errorStack = exceptionStack;
  return exceptionStack;
}

void MouseInteractable::flushErrorStack() { exceptionStack.clear(); }
