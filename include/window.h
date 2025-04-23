#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <map>
#include <vector>
#include <functional>
#include "exception.h"

// Forward declarations
class GUIInteractables;
class KeyboardEventHandler;
class MouseEventHandler;
class ControllerEventHandler;
class Window;

/**
 * @class GUIInteractables
 * @brief Represents a graphical user interface (GUI) interactable element.
 *
 * The `GUIInteractables` class provides a framework for managing GUI elements
 * that can interact with each other in a spatial hierarchy. Each instance of
 * this class can be linked to other instances in four directions: left, right,
 * up, and down. It also contains an SDL_Rect object to define its position and
 * dimensions, as well as an active state to indicate whether the element is
 * currently active.
 *
 * The class supports default copy and move constructors, as well as copy and
 * move assignment operators, allowing for efficient copying and transferring
 * of resources. It provides methods for retrieving neighboring elements,
 * managing the active state, and adding or searching for interactables within
 * the hierarchy.
 *
 * Key features:
 * - Spatial hierarchy management for GUI elements.
 * - Support for adding and searching interactables based on position.
 * - Active state management for enabling or disabling elements.
 * - Integration with SDL_Rect for defining position and dimensions.
 */
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
  /**
   * @brief Constructs a GUIInteractables object with specified properties.
   *
   * @param newRect Pointer to an SDL_Rect object defining the dimensions and position of the interactable element.
   * @param newfunc A function to be executed when the interactable element is triggered.
   *                The function takes a reference to a vector of void pointers as its argument.
   * @param button Optional parameter specifying the mouse state. Defaults to 1 (true).
   */
  GUIInteractables(SDL_Rect* newRect, const function<void(vector<void*>&)>& newfunc, const bool button = 1);

  /**
   * @brief Retrieves the SDL_Rect object associated with this instance.
   *
   * @param returnval Optional pointer to an SDL_Rect. If provided, it will be
   *                  set to the value of the internal rectangle.
   * @return SDL_Rect* Pointer to the internal SDL_Rect object.
   *
   * @note If `returnval` is not `nullptr`, it will be overwritten with the
   *       internal rectangle's value. The function always returns the internal
   *       rectangle pointer.
   */
  SDL_Rect* getRectangle(SDL_Rect* returnval = nullptr);

  /**
   * @brief Retrieves the left GUIInteractables object.
   *
   * This function returns the pointer to the left GUIInteractables object.
   * If a non-null pointer is passed as the parameter `returnval`, it will
   * be updated to point to the left GUIInteractables object as well.
   *
   * @param returnval Optional pointer to a GUIInteractables object. If not null,
   *                  it will be updated to point to the left GUIInteractables object.
   *                  Defaults to nullptr.
   *
   * @return GUIInteractables* Pointer to the left GUIInteractables object.
   */
  GUIInteractables* getLeft(GUIInteractables* returnval = nullptr);

  /**
   * @brief Retrieves the right GUIInteractables object.
   *
   * This function returns the pointer to the `right` GUIInteractables object.
   * If a non-null pointer is passed as the `returnval` parameter, it will be
   * updated to point to the same `right` object.
   *
   * @param returnval Optional pointer to a GUIInteractables object. If not null,
   *                  it will be set to the `right` object.
   * @return GUIInteractables* Pointer to the `right` GUIInteractables object.
   */
  GUIInteractables* getRight(GUIInteractables* returnval = nullptr);

  /**
   * @brief Retrieves the GUIInteractables object located above the current one.
   *
   * This function returns the pointer to the `up` GUIInteractables object.
   * Optionally, if a non-null pointer is passed as the `returnval` parameter,
   * it will be updated to point to the same `up` object.
   *
   * @param returnval Optional pointer to a GUIInteractables object. If not null,
   *                  it will be set to the `up` object.
   * @return GUIInteractables* Pointer to the `up` GUIInteractables object.
   */
  GUIInteractables* getUp(GUIInteractables* returnval = nullptr);

  /**
   * @brief Retrieves the GUIInteractables object located below the current one.
   *
   * This function returns the `down` pointer, which represents the GUIInteractables
   * object positioned below the current object in the GUI hierarchy. If a non-null
   * pointer is passed as the `returnval` parameter, it will be updated to point to
   * the same `down` object.
   *
   * @param returnval Optional pointer to a GUIInteractables object. If not null,
   *                  it will be updated to point to the `down` object.
   * @return GUIInteractables* Pointer to the `down` object.
   */
  GUIInteractables* getDown(GUIInteractables* returnval = nullptr);

  /**
   * @brief Adds a new GUIInteractable to the current GUI structure based on its position.
   *
   * This function organizes GUIInteractables into a spatial hierarchy based on their
   * positions relative to the current element. It determines whether the new interactable
   * should be placed to the left, right, above, or below the current element. If the
   * position overlaps or the element already exists, an exception is thrown.
   *
   * @param newInteractable Pointer to the new GUIInteractables object to be added.
   *
   * @throws Exception If the new interactable overlaps with an existing one or already exists.
   *
   * The placement logic is as follows:
   * - If the new interactable is to the left of the current element, it is added to the `left` child.
   * - If the new interactable is to the right of the current element, it is added to the `right` child.
   * - If the new interactable is above the current element, it is added to the `up` child.
   * - If the new interactable is below the current element, it is added to the `down` child.
   *
   * If the respective child node is already occupied, the function recursively calls
   * `addInteractable` on the corresponding child node.
   */
  void addInteractable(GUIInteractables* newInteractable);

  /**
   * @brief Finds a GUIInteractable item based on the given x and y coordinates.
   *
   * This function recursively searches for a GUIInteractable item within a
   * hierarchical structure of GUI elements. The search is performed based on
   * the provided x and y coordinates relative to the boundaries of the current
   * rectangle. If the coordinates fall outside the current rectangle, the
   * function navigates to the appropriate child node (left, right, up, or down)
   * to continue the search.
   *
   * @param x The x-coordinate of the point to search for.
   * @param y The y-coordinate of the point to search for.
   * @param returnval An optional pointer to a GUIInteractables object to store the result.
   * @return A pointer to the GUIInteractables object that matches the given
   *         coordinates, or nullptr if no matching item is found.
   */
  GUIInteractables* findItem(const int x, const int y, GUIInteractables* returnval);

  /**
   * @brief Retrieves the active state of the window.
   *
   * This function returns the current active state of the window. Optionally,
   * it can also set a pointer to the `active` member variable if a non-null
   * pointer is provided as an argument.
   *
   * @param returnval Optional pointer to a boolean. If not null, it will be
   *                  set to point to the `active` member variable.
   * @return true if the window is active, false otherwise.
   */
  bool getActive(bool* returnval = nullptr);

  /**
   * @brief Toggles the active state of the object.
   *
   * This function switches the `active` state between `true` and `false`.
   * If the object is currently active, it will be deactivated, and vice versa.
   */
  void toggleActive();

  /**
   * @brief Sets the active state of the window.
   *
   * @param state A boolean value indicating whether the window should be active (true) or inactive (false).
   */
  void setActive(const bool state);

  /**
   * @brief Updates the current function with a new function.
   *
   * This method allows you to replace the existing function with a new one.
   * The new function should accept a vector of void pointers as its parameter.
   *
   * @param newfunc A constant reference to the new function to be assigned.
   *                The function should have the signature `void(vector<void *> &)`.
   */
  void changeFunction(const function<void(vector<void*>&)>& newfunc);

  /**
   * @brief Changes the current mouse button state if the provided button is valid.
   *
   * This function updates the `mouseState` variable to the specified button value
   * if it matches one of the valid button identifiers (0, 1, or 2). If the provided
   * button value is invalid, the function does nothing.
   *
   * @param button The button identifier to set. Valid values are:
   *               - 0: Represents the left mouse button.
   *               - 1: Represents the middle mouse button.
   *               - 2: Represents the right mouse button.
   */
  void changeButton(const int button);

  /**
   * @brief Retrieves the current mouse button state and optionally updates the provided pointer.
   *
   * @param button A pointer to an integer where the address of the internal mouse button state
   *               will be stored. If nullptr is passed, no update is performed.
   * @return The current mouse button state as an integer.
   */
  bool getButton(bool* button = nullptr);

  /**
   * @brief Retrieves the stored function and optionally assigns it to the provided function reference.
   *
   * @param returnval An optional parameter. If provided and not null, it will be assigned the stored function.
   *                  Defaults to nullptr.
   * @return The stored function of type `function<void(vector<void *> &)>`.
   */
  function<void(vector<void*>&)> getFunction(function<void(vector<void*>&)> returnval = nullptr);
};

/**
 * @class KeyboardEventHandler
 * @brief Handles keyboard input events and bindings.
 */
class KeyboardEventHandler
{
private:
  map<SDL_KeyCode, pair<function<void(vector<void*>&)>, pair<vector<void*>, bool>>> keys;

public:
  /**
   * @brief Default copy constructor for the KeyboardEventHandler class.
   *
   * This constructor creates a new instance of KeyboardEventHandler by copying
   * the state of an existing instance. It performs a member-wise copy of all
   * fields in the class.
   */
  KeyboardEventHandler(const KeyboardEventHandler&) = default;

  /**
   * @brief Move constructor for the KeyboardEventHandler class.
   *
   * This constructor allows the transfer of resources from another
   * KeyboardEventHandler instance to this instance. It is marked as
   * `default`, meaning the compiler will generate the implementation
   * automatically.
   */
  KeyboardEventHandler(KeyboardEventHandler&&) = default;

  /**
   * @brief Default assignment operator for the KeyboardEventHandler class.
   *
   * This operator allows assigning one KeyboardEventHandler object to another
   * using the default behavior provided by the compiler. It performs a member-wise
   * assignment of all non-static members.
   *
   * @return A reference to the assigned KeyboardEventHandler object.
   */
  KeyboardEventHandler& operator=(const KeyboardEventHandler&) = default;

  /**
   * @brief Default move assignment operator for the KeyboardEventHandler class.
   *
   * This operator allows the assignment of a KeyboardEventHandler object
   * using move semantics. The default implementation is used, which performs
   * a member-wise move of the object's resources.
   *
   * @return A reference to the current instance after the move assignment.
   */
  KeyboardEventHandler& operator=(KeyboardEventHandler&&) = default;

  /**
   * @brief Default constructor for the KeyboardEventHandler class.
   *
   * This constructor initializes a KeyboardEventHandler object.
   * It does not perform any specific actions or initialization logic.
   */
  KeyboardEventHandler() {}

  /**
   * @brief Adds a keyboard input handler for a specific key.
   *
   * This function associates a callback function with a specific keyboard key.
   * The callback will be triggered when the key is pressed or released,
   * depending on the specified state.
   *
   * @param key The SDL_KeyCode representing the key to associate the function
   * with.
   * @param func The callback function to execute when the key event occurs.
   *             It takes a vector of void pointers as an argument.
   * @param extradata Optional additional data to pass to the callback function.
   *                  Defaults to an empty vector.
   * @param state The state of the key to trigger the callback.
   *              Use 0 for key release and 1 for key press. Defaults to 0.
   *
   * @throws Exception If the specified key already has an assigned function.
   */
  void addKeyboardInput(const SDL_KeyCode key, const function<void(vector<void*>&)>& func, vector<void*> extradata = {}, const bool state = 0); // state 0 = Released, 1 = Pressed

  /**
   * @brief Modifies the behavior of a keyboard input binding.
   *
   * This function allows you to update the function, extra data, or state associated
   * with a specific keyboard key binding. If the key is not already bound, an exception
   * is thrown.
   *
   * @param key The SDL_KeyCode representing the key to modify.
   * @param newfunc A function to replace the current behavior of the key. Defaults to NULL,
   *                which means the function will not be modified.
   * @param extradata A vector of additional data to associate with the key. Defaults to an
   *                  empty vector, which means the extra data will not be modified.
   * @param state The new state of the key binding. Acceptable values are:
   *              - 0: Released
   *              - 1: Pressed
   *              - -1: No change (default)
   *
   * @throws Exception If the key is not already bound, an exception is thrown with an
   *                   appropriate error message.
   */
  void modifyKeyboardInput(const SDL_KeyCode key, const function<void(vector<void*>&)>& newfunc = NULL, vector<void*> extradata = {}, const int state = -1);

  /**
   * @brief Removes a specific keyboard input from the tracked keys.
   *
   * This function removes the specified key from the internal collection
   * of tracked keys, effectively disabling its input handling.
   *
   * @param key The SDL_KeyCode representing the key to be removed.
   */
  void removeKeyboardInput(const SDL_KeyCode key);

  /**
   * @brief Handles keyboard input events and processes them accordingly.
   *
   * This function processes SDL keyboard events in a loop, checking for specific
   * key combinations and executing associated actions. It also updates the state
   * of modifier keys (Ctrl, Alt, Shift) and handles custom key bindings stored
   * in the `keys` map.
   *
   * @param event Pointer to an SDL_Event structure containing the event data.
   *
   * Key functionalities:
   * - Detects and handles the SDL_QUIT event to terminate the application.
   * - Tracks the state of modifier keys (left/right Ctrl, Alt, Shift).
   * - Detects the Alt+F4 key combination to terminate the application.
   * - Executes custom actions for keys found in the `keys` map if their state
   *   matches the expected state.
   */
  void cycleKeyboardInputs(SDL_Event* event);
};

/**
 * @class MouseEventHandler
 * @brief Handles mouse input events and GUI interactions.
 */
class MouseEventHandler
{
private:
  map<SDL_Rect*, function<void(vector<void*>&)>> lHoldGameInt, lPushGameInt, rHoldGameInt, rPushGameInt, mHoldGameInt, mPushGameInt;
  map<int, GUIInteractables*> guis;

public:
  /**
   * @brief Adds a GUI interactable element to the collection and assigns it a unique identifier (UUID).
   *
   * @param firstElement A pointer to the GUIInteractables object to be added.
   * @return int The unique identifier (UUID) assigned to the added GUI element.
   */
  int addGUI(GUIInteractables* firstElement);

  /**
   * @brief Adds a game interactable to the appropriate container based on mouse state and button.
   *
   * This function associates a rectangle with a callback function and stores it in a specific
   * container depending on whether the mouse state is "hold" or "push" and which mouse button
   * is being used (left, middle, or right). If the rectangle already exists in the target
   * container, an exception is thrown.
   *
   * @param rectangle Pointer to an SDL_Rect representing the interactable area.
   * @param func A callback function to be executed when the interactable is triggered.
   * @param mouseState A boolean indicating the mouse state:
   *                   - true: "hold" state (mouse button is held down).
   *                   - false: "push" state (mouse button is clicked).
   * @param mouseButton An integer representing the mouse button:
   *                    - 0: Left mouse button.
   *                    - 1: Middle mouse button.
   *                    - 2: Right mouse button.
   *
   * @throws Exception If the rectangle already exists in the target container.
   * @throws Exception If an invalid mouse button is provided.
   */
  void addGameInteractable(SDL_Rect* rectangle, const function<void(vector<void*>&)>& func, const bool mouseState = 0, const int mouseButton = 0);

  void cycleMouseEvents(SDL_Event* event)
  {
    while (SDL_PollEvent(event))
    {
      if (event->button.button == SDL_BUTTON_LEFT && event->type == SDL_MOUSEBUTTONDOWN)
      {
      }
    }
  }
};

/**
 * @class ControllerEventHandler
 * @brief Placeholder for controller input handling (not implemented).
 */
class ControllerEventHandler
{
private:
public:
};

/**
 * @class Window
 * @brief Manages the SDL window, renderer, and application state.
 */
class Window
{
private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  bool execute = true;
  vector<Exception> exceptionStack;

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
   * @param title The title of the window. Defaults to empty
   * @param width The width of the window in pixels. Defaults to 1280.
   * @param height The height of the window in pixels. Defaults to 720.
   * @param flags The flags to use. Allows for piping. Defaults to SDL_WINDOW_SHOWN
   *
   * @throws Exception If the SDL window or renderer fails to initialize.
   */
  Window(const char* title = "", const int width = 1280, const int height = 720, uint32_t flags = SDL_WINDOW_SHOWN);

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
  const bool isRunning(SDL_Event* event, bool* returnval = nullptr);

  /**
   * @brief Replaces the current SDL_Renderer with a new one.
   *
   * This function attempts to replace the current renderer with a new renderer.
   * If the operation fails at any step, it ensures the old renderer is restored
   * to its previous state and throws an exception with detailed error information.
   *
   * @param newRenderer A pointer to the new SDL_Renderer to replace the current one.
   * @throws Exception if:
   *         - The old renderer cannot be saved to a temporary location.
   *         - The new renderer cannot replace the old renderer.
   *         - The old renderer cannot be restored after a failure.
   *         - The old renderer cannot be destroyed, potentially causing memory leaks.
   *
   * @note The function uses exception handling to manage errors and ensure
   *       the integrity of the renderer state.
   */
  void replaceRenderer(SDL_Renderer* newRenderer);

  /**
   * @brief Replaces the current SDL_Renderer with a new one.
   *
   * This function attempts to replace the current renderer with a new renderer.
   * If the operation fails at any step, it ensures the old renderer is restored
   * to its previous state and throws an exception with detailed error information.
   *
   * @param newRenderer A pointer to the new SDL_Renderer to replace the current one.
   * @throws Exception if:
   *         - The old renderer cannot be saved to a temporary location.
   *         - The new renderer cannot replace the old renderer.
   *         - The old renderer cannot be restored after a failure.
   *         - The old renderer cannot be destroyed, potentially causing memory leaks.
   *
   * @note The function uses exception handling to manage errors and ensure
   *       the integrity of the renderer state.
   */
  void replaceWindow(SDL_Window* newWindow);

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

  /**
   * @brief Changes the title of the window.
   *
   * This function sets a new title for the window and verifies that the title
   * was successfully updated. If the title change fails, an exception is thrown.
   *
   * @param newTitle The new title to set for the window. Must be a null-terminated string.
   *
   * @throws Exception If the window title could not be changed. The exception
   *         contains the error message, the function name, and error codes.
   */
  void changeTitle(const char* newTitle);

  /**
   * @brief Changes the resolution of the SDL window.
   *
   * This function adjusts the width and height of the SDL window. If either
   * the width or height is set to 0, the current dimension for that parameter
   * will be retained.
   *
   * @param width The new width of the window. Defaults to 0, which retains the current width.
   * @param height The new height of the window. Defaults to 0, which retains the current height.
   *
   * @note The function retrieves the current window size using SDL_GetWindowSize
   *       and updates it with SDL_SetWindowSize. Ensure that the `window` object
   *       is properly initialized before calling this function.
   */
  void changeResolution(const int width = 0, const int height = 0);

  /**
   * @brief Updates the window flags for an SDL window.
   *
   * This function modifies the properties of the SDL window based on the provided
   * flags. It checks the current state of the window and applies changes accordingly.
   *
   * @param flags A bitmask of SDL window flags to apply. The following flags are supported:
   *              - SDL_WINDOW_FULLSCREEN: Sets the window to fullscreen mode.
   *              - SDL_WINDOW_FULLSCREEN_DESKTOP: Sets the window to fullscreen desktop mode.
   *              - SDL_WINDOW_SHOWN: Makes the window visible.
   *              - SDL_WINDOW_HIDDEN: Hides the window.
   *              - SDL_WINDOW_BORDERLESS: Toggles the borderless state of the window.
   *              - SDL_WINDOW_RESIZABLE: Toggles the resizable state of the window.
   *
   * The function uses SDL_GetWindowFlags to retrieve the current state of the window
   * and applies changes only if necessary. For SDL_WINDOW_BORDERLESS and SDL_WINDOW_RESIZABLE,
   * the function toggles the state based on the current flag state.
   */
  void updateFlags(uint32_t flags);

  /**
   * @brief Retrieves the current stack of exceptions.
   *
   * This function returns the current exception stack and optionally copies it
   * into a provided stack. If a non-null pointer is passed as the parameter,
   * the exception stack is copied into the provided stack.
   *
   * @param stack A pointer to a vector of Exception objects. If not null,
   *              the current exception stack will be copied into this vector.
   *              Defaults to nullptr.
   * @return A vector containing the current exception stack.
   */
  vector<Exception> errorStack(vector<Exception>* stack = nullptr);

  /**
   * @brief Clears all exceptions from the error stack.
   *
   * This function empties the `exceptionStack`, removing all stored exceptions.
   * It is useful for resetting the error state or preparing the stack for new
   * error handling operations.
   */
  void flushErrorStack();
};

#endif