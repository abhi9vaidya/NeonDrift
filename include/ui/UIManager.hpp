#pragma once

#include "core/GameState.hpp"
#include "core/ScoreManager.hpp"
#include <SFML/Graphics.hpp>
#include <string>


/**
 * Manages all UI elements: HUD, menus, and overlays
 */
class UIManager {
public:
  UIManager();

  // Initialize with window size
  bool init(unsigned int windowWidth, unsigned int windowHeight);

  // Update animations
  void update(float deltaTime);

  // Render based on game state
  void renderHUD(sf::RenderWindow &window, const ScoreManager &score,
                 float playerSpeed);
  void renderMenu(sf::RenderWindow &window);
  void renderPauseOverlay(sf::RenderWindow &window);
  void renderGameOver(sf::RenderWindow &window, const ScoreManager &score);

private:
  void drawComboMeter(sf::RenderWindow &window, const ScoreManager &score);
  void drawSpeedometer(sf::RenderWindow &window, float speed);
  std::string formatScore(std::uint64_t score);

  // Font
  sf::Font m_font;
  bool m_fontLoaded;

  // Window dimensions
  unsigned int m_windowWidth;
  unsigned int m_windowHeight;

  // UI animation
  float m_menuPulse;

  // Neon colors
  sf::Color m_neonCyan;
  sf::Color m_neonMagenta;
  sf::Color m_neonWhite;
};
