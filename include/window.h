#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include "exception.h"

/**
 * @class Window
 * @brief A class that encapsulates an SDL window and renderer, providing utility functions for managing them.
 *
 * The Window class is designed to simplify the creation and management of SDL windows and renderers.
 * It includes functionality to retrieve the renderer and window, check if the window is running, and
 * replace the renderer or window in niche cases.
 *
 * @note The class disables move semantics to ensure proper resource management.
 */
class Window
{
private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool execute = true;

protected:
public:
  /**
   * @brief Default copy constructor for the Window class.
   *
   * This constructor performs a member-wise copy of the Window object,
   * copying all non-static members from the source object to the new object.
   */
  Window(const Window&) = default;

  /**
   * @brief Deleted move constructor to prevent moving of Window objects.
   *
   * This ensures that Window instances cannot be moved, which might be
   * necessary to maintain resource ownership or other invariants.
   */
  Window(Window&&) = delete;

  /**
   * @brief Default copy assignment operator for the Window class.
   *
   * This operator allows assigning one Window object to another using the default
   * implementation provided by the compiler. It performs a shallow copy of all
   * non-static members of the Window class.
   *
   * @param other The Window object to copy from.
   * @return A reference to the current Window object after assignment.
   */
  Window& operator=(const Window&) = default;

  /**
   * @brief Deleted move assignment operator for the Window class.
   *
   * This prevents the Window object from being moved using the assignment operator.
   * It ensures that the Window instance cannot be reassigned from another instance
   * through move semantics, preserving its integrity and preventing unintended behavior.
   */
  Window& operator=(Window&&) = delete;


  /**
   * @brief Constructs a Window object and initializes an SDL window and renderer.
   *
   * This constructor creates an SDL window with the specified title, width, and height.
   * It also initializes an SDL renderer for the created window. If either the window
   * or the renderer fails to initialize, an Exception is thrown.
   *
   * @param title The title of the window.
   * @param width The width of the window in pixels. Defaults to 1280.
   * @param height The height of the window in pixels. Defaults to 720.
   *
   * @throws Exception If the SDL window or renderer fails to initialize.
   */
  Window(const char* title, const int width = 1280, const int height = 720);

  /**
   * @brief Destructor for the Window class.
   *
   * Cleans up resources associated with the Window object by destroying
   * the SDL renderer and SDL window. This ensures proper deallocation
   * of memory and prevents resource leaks.
   */
  ~Window();

  /**
   * @brief Retrieves the SDL_Renderer instance associated with the game.
   *
   * @param returnval Optional parameter. If not nullptr, it will be assigned
   *                  the value of the internal renderer. Defaults to nullptr.
   * @return SDL_Renderer* Pointer to the internal SDL_Renderer instance.
   */
  SDL_Renderer* getRenderer(SDL_Renderer* returnval = nullptr);

  /**
   * @brief Retrieves the SDL_Window instance associated with the game.
   *
   * This function returns the current SDL_Window instance. If a non-null
   * pointer is passed as the parameter `returnval`, it will be assigned
   * the value of the current window before returning the window.
   *
   * @param returnval Optional pointer to an SDL_Window. If not null, it will
   *                   be assigned the value of the current window.
   * @return SDL_Window* The current SDL_Window instance.
   */
  SDL_Window* getWindow(SDL_Window* returnval = nullptr);

  /**
   * @brief Checks if the game is currently running and optionally sets a return value.
   *
   * @param returnval Optional pointer to a boolean. If provided, it will be set to the
   *                  current state of the game (true if running, false otherwise).
   * @return true if the game is running, false otherwise.
   */
  const bool isRunning(bool* returnval = nullptr);

  /**
   * @brief Replaces the current SDL_Renderer with a new one.
   *
   * This function attempts to replace the current renderer with a new renderer.
   * If the operation fails at any step, it ensures the old renderer is restored
   * to its previous state and throws an exception with detailed error information.
   *
   * @param newRenderer A pointer to the new SDL_Renderer to replace the current one.
   * @return true if the renderer was successfully replaced.
   * @throws Exception if:
   *         - The old renderer cannot be saved to a temporary location.
   *         - The new renderer cannot replace the old renderer.
   *         - The old renderer cannot be restored after a failure.
   *         - The old renderer cannot be destroyed, potentially causing memory leaks.
   *
   * @note The function uses exception handling to manage errors and ensure
   *       the integrity of the renderer state.
   */
  const bool replaceRenderer(SDL_Renderer* newRenderer);

  /**
   * @brief Replaces the current SDL_Renderer with a new one.
   *
   * This function attempts to replace the current renderer with a new renderer.
   * If the operation fails at any step, it ensures the old renderer is restored
   * to its previous state and throws an exception with detailed error information.
   *
   * @param newRenderer A pointer to the new SDL_Renderer to replace the current one.
   * @return true if the renderer was successfully replaced.
   * @throws Exception if:
   *         - The old renderer cannot be saved to a temporary location.
   *         - The new renderer cannot replace the old renderer.
   *         - The old renderer cannot be restored after a failure.
   *         - The old renderer cannot be destroyed, potentially causing memory leaks.
   *
   * @note The function uses exception handling to manage errors and ensure
   *       the integrity of the renderer state.
   */
  const bool replaceWindow(SDL_Window* newWindow);

  /**
   * @brief Toggles the execution state.
   *
   * This function switches the value of the `execute` variable
   * between `true` and `false`. If `execute` is currently `true`,
   * it will be set to `false`, and vice versa.
   */
  void swapExecution();

  /**
   * @brief Sets the execution state of the game.
   *
   * @param state A boolean value indicating the desired execution state.
   *              Pass `true` to enable execution, or `false` to disable it.
   */
  void setExecution(const bool state);

  /**
   * @brief Terminates the execution of the program by setting the execute flag to false.
   */
  void endExecution();
};
#endif