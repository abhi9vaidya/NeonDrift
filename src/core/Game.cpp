#include "core/Game.hpp"
#include <algorithm>

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Neon Drift",
               sf::Style::Close | sf::Style::Titlebar),
      m_currentState(GameState::Menu), m_pendingState(GameState::Menu),
      m_stateChangeRequested(false), m_accumulator(0.0f) {
  m_window.setFramerateLimit(60);
}

void Game::run() {
  while (m_window.isOpen()) {
    // Calculate delta time
    float deltaTime = m_clock.restart().asSeconds();

    // Cap delta time to prevent spiral of death
    deltaTime = std::min(deltaTime, 0.25f);

    // Process window events
    processEvents();

    // Handle any pending state changes
    handleStateTransition();

    // Fixed timestep physics update
    m_accumulator += deltaTime;
    while (m_accumulator >= FIXED_TIMESTEP) {
      update(FIXED_TIMESTEP);
      m_accumulator -= FIXED_TIMESTEP;
    }

    // Render
    render();
  }
}

void Game::processEvents() {
  while (const auto event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      m_window.close();
      return;
    }

    // Key pressed
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      m_inputManager.keyPressed(keyPressed->code);

      // Escape key handling based on state
      if (keyPressed->code == sf::Keyboard::Key::Escape) {
        switch (m_currentState) {
        case GameState::Menu:
          m_window.close();
          break;
        case GameState::Playing:
          m_pendingState = GameState::Paused;
          m_stateChangeRequested = true;
          break;
        case GameState::Paused:
          m_pendingState = GameState::Playing;
          m_stateChangeRequested = true;
          break;
        case GameState::GameOver:
          m_pendingState = GameState::Menu;
          m_stateChangeRequested = true;
          break;
        }
      }

      // Enter to start game from menu
      if (keyPressed->code == sf::Keyboard::Key::Enter) {
        if (m_currentState == GameState::Menu) {
          m_pendingState = GameState::Playing;
          m_stateChangeRequested = true;
        }
        if (m_currentState == GameState::GameOver) {
          m_player.reset();
          m_pendingState = GameState::Playing;
          m_stateChangeRequested = true;
        }
      }
    }

    // Key released
    if (const auto *keyReleased = event->getIf<sf::Event::KeyReleased>()) {
      m_inputManager.keyReleased(keyReleased->code);
    }
  }
}

void Game::handleStateTransition() {
  if (m_stateChangeRequested) {
    m_currentState = m_pendingState;
    m_stateChangeRequested = false;

    // Clear input on state change to prevent stuck keys
    m_inputManager.clear();
  }
}

void Game::update(float deltaTime) {
  switch (m_currentState) {
  case GameState::Menu:
    // Menu update logic (animations, etc.)
    break;

  case GameState::Playing:
    m_player.update(deltaTime, m_inputManager);
    break;

  case GameState::Paused:
    // Paused - no game updates
    break;

  case GameState::GameOver:
    // Game over screen logic
    break;
  }
}

void Game::render() {
  // Clear with deep purple/black neon background
  m_window.clear(sf::Color(15, 5, 25));

  switch (m_currentState) {
  case GameState::Menu:
    // TODO: Render menu UI
    break;

  case GameState::Playing:
    m_player.render(m_window);
    break;

  case GameState::Paused:
    // Render game world (frozen) + pause overlay
    m_player.render(m_window);
    break;

  case GameState::GameOver:
    // TODO: Render game over screen
    break;
  }

  m_window.display();
}
