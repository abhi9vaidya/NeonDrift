#pragma once

#include "core/GameState.hpp"
#include "core/InputManager.hpp"
#include <SFML/Graphics.hpp>


/**
 * Main Game class
 * Manages window, game loop, and state transitions
 */
class Game {
public:
  Game();
  ~Game() = default;

  // Main entry point - runs the game
  void run();

private:
  // Core loop methods
  void processEvents();
  void update(float deltaTime);
  void render();

  // State handling
  void handleStateTransition();

  // Window
  sf::RenderWindow m_window;

  // Game state
  GameState m_currentState;
  GameState m_pendingState;
  bool m_stateChangeRequested;

  // Input
  InputManager m_inputManager;

  // Timing
  sf::Clock m_clock;
  static constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
  float m_accumulator;

  // Window settings
  static constexpr unsigned int WINDOW_WIDTH = 1280;
  static constexpr unsigned int WINDOW_HEIGHT = 720;
};
