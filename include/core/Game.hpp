#pragma once

#include "core/GameState.hpp"
#include "core/InputManager.hpp"
#include "core/ScoreManager.hpp"
#include "entities/Player.hpp"
#include "graphics/ParticleSystem.hpp"
#include "ui/UIManager.hpp"
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

  // Game entities
  Player m_player;

  // Visual effects
  ParticleSystem m_particles;
  sf::Vector2f m_screenShake;
  float m_shakeIntensity;

  // Scoring and UI
  ScoreManager m_scoreManager;
  UIManager m_uiManager;
  bool m_wasDrifting;

  // Timing
  sf::Clock m_clock;
  static constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
  float m_accumulator;

  // Window settings
  static constexpr unsigned int WINDOW_WIDTH = 1280;
  static constexpr unsigned int WINDOW_HEIGHT = 720;
};
