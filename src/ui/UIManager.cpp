#include "ui/UIManager.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>


UIManager::UIManager()
    : m_fontLoaded(false), m_windowWidth(1280), m_windowHeight(720),
      m_menuPulse(0.0f), m_neonCyan(0, 255, 255), m_neonMagenta(255, 0, 255),
      m_neonWhite(240, 240, 255) {}

bool UIManager::init(unsigned int windowWidth, unsigned int windowHeight) {
  m_windowWidth = windowWidth;
  m_windowHeight = windowHeight;

  // Try to load a font - if not available, we'll use basic shapes
  if (m_font.openFromFile("assets/fonts/Orbitron-Regular.ttf")) {
    m_fontLoaded = true;
  } else if (m_font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
    m_fontLoaded = true;
  }

  return true;
}

void UIManager::update(float deltaTime) {
  m_menuPulse += deltaTime * 2.0f;
  if (m_menuPulse > 6.28318f)
    m_menuPulse -= 6.28318f;
}

std::string UIManager::formatScore(std::uint64_t score) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(8) << score;
  return ss.str();
}

void UIManager::renderHUD(sf::RenderWindow &window, const ScoreManager &score,
                          float playerSpeed) {
  if (!m_fontLoaded)
    return;

  // Score display (top left)
  sf::Text scoreText(m_font, "SCORE: " + formatScore(score.getScore()), 28);
  scoreText.setPosition(sf::Vector2f(20.0f, 20.0f));
  scoreText.setFillColor(m_neonCyan);
  scoreText.setOutlineColor(sf::Color(0, 100, 100));
  scoreText.setOutlineThickness(2.0f);
  window.draw(scoreText);

  // Combo multiplier (top left, below score)
  if (score.isComboActive()) {
    std::stringstream comboSS;
    comboSS << "x" << std::fixed << std::setprecision(1)
            << score.getComboMultiplier();

    sf::Text comboText(m_font, comboSS.str(), 36);
    comboText.setPosition(sf::Vector2f(20.0f, 55.0f));

    // Pulse color based on multiplier
    float pulse = (std::sin(m_menuPulse * 3.0f) + 1.0f) * 0.5f;
    float multiplier = score.getComboMultiplier();
    std::uint8_t r =
        static_cast<std::uint8_t>(255 * std::min(1.0f, multiplier / 4.0f));
    std::uint8_t g = static_cast<std::uint8_t>(255 - r);
    comboText.setFillColor(
        sf::Color(r, g, 255, static_cast<std::uint8_t>(200 + 55 * pulse)));
    comboText.setOutlineThickness(1.0f);
    window.draw(comboText);
  }

  // Draw combo timer bar
  drawComboMeter(window, score);

  // Speed display (bottom right)
  drawSpeedometer(window, playerSpeed);
}

void UIManager::drawComboMeter(sf::RenderWindow &window,
                               const ScoreManager &score) {
  float barWidth = 200.0f;
  float barHeight = 8.0f;
  float x = 20.0f;
  float y = 100.0f;

  // Background bar
  sf::RectangleShape bgBar(sf::Vector2f(barWidth, barHeight));
  bgBar.setPosition(sf::Vector2f(x, y));
  bgBar.setFillColor(sf::Color(40, 40, 60, 150));
  bgBar.setOutlineColor(sf::Color(80, 80, 120));
  bgBar.setOutlineThickness(1.0f);
  window.draw(bgBar);

  // Combo timer fill
  if (score.isComboActive()) {
    float fillRatio = score.getComboTimer() / score.getMaxComboTimer();
    sf::RectangleShape fillBar(sf::Vector2f(barWidth * fillRatio, barHeight));
    fillBar.setPosition(sf::Vector2f(x, y));

    // Color based on time remaining
    std::uint8_t r = static_cast<std::uint8_t>(255 * (1.0f - fillRatio));
    std::uint8_t g = static_cast<std::uint8_t>(255 * fillRatio);
    fillBar.setFillColor(sf::Color(r, g, 200));
    window.draw(fillBar);
  }
}

void UIManager::drawSpeedometer(sf::RenderWindow &window, float speed) {
  if (!m_fontLoaded)
    return;

  std::stringstream ss;
  ss << static_cast<int>(speed) << " km/h";

  sf::Text speedText(m_font, ss.str(), 32);

  // Position bottom right
  sf::FloatRect bounds = speedText.getLocalBounds();
  speedText.setPosition(sf::Vector2f(m_windowWidth - bounds.size.x - 30.0f,
                                     m_windowHeight - 50.0f));

  // Color based on speed (green to red)
  float speedRatio = std::min(1.0f, speed / 600.0f);
  std::uint8_t r = static_cast<std::uint8_t>(100 + 155 * speedRatio);
  std::uint8_t g = static_cast<std::uint8_t>(255 * (1.0f - speedRatio * 0.5f));
  speedText.setFillColor(sf::Color(r, g, 255));
  speedText.setOutlineColor(sf::Color(r / 4, g / 4, 100));
  speedText.setOutlineThickness(2.0f);

  window.draw(speedText);
}

void UIManager::renderMenu(sf::RenderWindow &window) {
  if (!m_fontLoaded)
    return;

  // Title
  sf::Text title(m_font, "NEON DRIFT", 72);
  sf::FloatRect titleBounds = title.getLocalBounds();
  title.setPosition(sf::Vector2f((m_windowWidth - titleBounds.size.x) / 2.0f,
                                 m_windowHeight * 0.25f));

  // Pulsing glow effect
  float pulse = (std::sin(m_menuPulse) + 1.0f) * 0.5f;
  title.setFillColor(
      sf::Color(static_cast<std::uint8_t>(200 + 55 * pulse), 255, 255));
  title.setOutlineColor(m_neonMagenta);
  title.setOutlineThickness(3.0f);
  window.draw(title);

  // Press Enter to start
  sf::Text startText(m_font, "Press ENTER to Start", 28);
  sf::FloatRect startBounds = startText.getLocalBounds();
  startText.setPosition(sf::Vector2f(
      (m_windowWidth - startBounds.size.x) / 2.0f, m_windowHeight * 0.55f));
  startText.setFillColor(
      sf::Color(255, 255, 255, static_cast<std::uint8_t>(150 + 105 * pulse)));
  window.draw(startText);

  // Controls hint
  sf::Text controlsText(
      m_font, "WASD/Arrows to move | SPACE to drift | ESC to pause", 18);
  sf::FloatRect controlsBounds = controlsText.getLocalBounds();
  controlsText.setPosition(sf::Vector2f(
      (m_windowWidth - controlsBounds.size.x) / 2.0f, m_windowHeight * 0.85f));
  controlsText.setFillColor(sf::Color(150, 150, 180));
  window.draw(controlsText);
}

void UIManager::renderPauseOverlay(sf::RenderWindow &window) {
  // Semi-transparent overlay
  sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(m_windowWidth),
                                          static_cast<float>(m_windowHeight)));
  overlay.setFillColor(sf::Color(10, 5, 20, 180));
  window.draw(overlay);

  if (!m_fontLoaded)
    return;

  // Paused text
  sf::Text pausedText(m_font, "PAUSED", 64);
  sf::FloatRect bounds = pausedText.getLocalBounds();
  pausedText.setPosition(sf::Vector2f((m_windowWidth - bounds.size.x) / 2.0f,
                                      m_windowHeight * 0.4f));
  pausedText.setFillColor(m_neonCyan);
  pausedText.setOutlineColor(m_neonMagenta);
  pausedText.setOutlineThickness(2.0f);
  window.draw(pausedText);

  // Resume hint
  sf::Text resumeText(m_font, "Press ESC to Resume", 24);
  sf::FloatRect resumeBounds = resumeText.getLocalBounds();
  resumeText.setPosition(sf::Vector2f(
      (m_windowWidth - resumeBounds.size.x) / 2.0f, m_windowHeight * 0.55f));
  resumeText.setFillColor(sf::Color(200, 200, 220));
  window.draw(resumeText);
}

void UIManager::renderGameOver(sf::RenderWindow &window,
                               const ScoreManager &score) {
  // Dark overlay
  sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(m_windowWidth),
                                          static_cast<float>(m_windowHeight)));
  overlay.setFillColor(sf::Color(20, 5, 30, 200));
  window.draw(overlay);

  if (!m_fontLoaded)
    return;

  // Game Over text
  sf::Text gameOverText(m_font, "GAME OVER", 72);
  sf::FloatRect bounds = gameOverText.getLocalBounds();
  gameOverText.setPosition(sf::Vector2f((m_windowWidth - bounds.size.x) / 2.0f,
                                        m_windowHeight * 0.25f));
  gameOverText.setFillColor(sf::Color(255, 50, 100));
  gameOverText.setOutlineColor(sf::Color(150, 0, 50));
  gameOverText.setOutlineThickness(3.0f);
  window.draw(gameOverText);

  // Final score
  sf::Text scoreText(m_font, "FINAL SCORE: " + formatScore(score.getScore()),
                     36);
  sf::FloatRect scoreBounds = scoreText.getLocalBounds();
  scoreText.setPosition(sf::Vector2f(
      (m_windowWidth - scoreBounds.size.x) / 2.0f, m_windowHeight * 0.45f));
  scoreText.setFillColor(m_neonCyan);
  window.draw(scoreText);

  // Restart hint
  float pulse = (std::sin(m_menuPulse * 2.0f) + 1.0f) * 0.5f;
  sf::Text restartText(m_font, "Press ENTER to Play Again", 28);
  sf::FloatRect restartBounds = restartText.getLocalBounds();
  restartText.setPosition(sf::Vector2f(
      (m_windowWidth - restartBounds.size.x) / 2.0f, m_windowHeight * 0.65f));
  restartText.setFillColor(
      sf::Color(255, 255, 255, static_cast<std::uint8_t>(150 + 105 * pulse)));
  window.draw(restartText);
}
