#include "core/Game.hpp"
#include <algorithm>
#include <cmath>
#include <random>

static std::random_device s_rd;
static std::mt19937 s_rng(s_rd());
static std::uniform_real_distribution<float> s_randFloat(-1.0f, 1.0f);

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Neon Drift",
               sf::Style::Close | sf::Style::Titlebar),
      m_currentState(GameState::Menu), m_pendingState(GameState::Menu),
      m_stateChangeRequested(false), m_screenShake(0.0f, 0.0f),
      m_shakeIntensity(0.0f), m_wasDrifting(false), m_accumulator(0.0f) {
  m_window.setFramerateLimit(60);
  m_uiManager.init(WINDOW_WIDTH, WINDOW_HEIGHT);
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
          m_scoreManager.reset();
          m_particles.clear();
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
  // Update screen shake
  if (m_shakeIntensity > 0.0f) {
    m_shakeIntensity *= 0.9f; // Decay
    m_screenShake.x = s_randFloat(s_rng) * m_shakeIntensity;
    m_screenShake.y = s_randFloat(s_rng) * m_shakeIntensity;
    if (m_shakeIntensity < 0.5f)
      m_shakeIntensity = 0.0f;
  }

  // Always update particles (even when paused for visual effect)
  m_particles.update(deltaTime);

  switch (m_currentState) {
  case GameState::Menu:
    m_uiManager.update(deltaTime);
    break;

  case GameState::Playing:
    m_player.update(deltaTime, m_inputManager);
    m_uiManager.update(deltaTime);

    // Update scoring
    m_scoreManager.update(deltaTime, m_player.getSpeed(), m_player.isDrifting(),
                          m_player.getDriftAmount());

    // Detect drift end for bonus
    if (m_wasDrifting && !m_player.isDrifting()) {
      m_scoreManager.onDriftEnd(m_player.getDriftAmount() * 2.0f,
                                m_player.getSpeed());
    }
    m_wasDrifting = m_player.isDrifting();

    // Emit drift particles when drifting
    if (m_player.isDrifting() && m_player.getSpeed() > 100.0f) {
      m_particles.emitDriftTrail(m_player.getPosition(), m_player.getVelocity(),
                                 m_player.getDriftAmount(), 0.0f);
    }

    // Emit speed lines at high speed
    m_particles.emitSpeedLines(m_player.getPosition(), m_player.getSpeed(),
                               m_player.getRotation());
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

  // Create view with screen shake offset
  sf::View view = m_window.getDefaultView();
  view.move(m_screenShake);
  m_window.setView(view);

  switch (m_currentState) {
  case GameState::Menu:
    m_uiManager.renderMenu(m_window);
    break;

  case GameState::Playing:
    m_particles.render(m_window);
    m_player.render(m_window);
    m_uiManager.renderHUD(m_window, m_scoreManager, m_player.getSpeed());
    break;

  case GameState::Paused:
    // Render game world (frozen) + pause overlay
    m_particles.render(m_window);
    m_player.render(m_window);
    m_uiManager.renderHUD(m_window, m_scoreManager, m_player.getSpeed());
    m_uiManager.renderPauseOverlay(m_window);
    break;

  case GameState::GameOver:
    m_particles.render(m_window);
    m_player.render(m_window);
    m_uiManager.renderGameOver(m_window, m_scoreManager);
    break;
  }

  // Reset view
  m_window.setView(m_window.getDefaultView());

  m_window.display();
}
