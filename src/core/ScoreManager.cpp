#include "core/ScoreManager.hpp"
#include <algorithm>
#include <cmath>

ScoreManager::ScoreManager()
    : m_score(0), m_comboMultiplier(1.0f), m_comboTimer(0.0f),
      m_driftMeter(0.0f), m_difficulty(0.0f), m_gameTime(0.0f) {}

void ScoreManager::reset() {
  m_score = 0;
  m_comboMultiplier = 1.0f;
  m_comboTimer = 0.0f;
  m_driftMeter = 0.0f;
  m_difficulty = 0.0f;
  m_gameTime = 0.0f;
}

void ScoreManager::update(float deltaTime, float playerSpeed, bool isDrifting,
                          float driftAmount) {
  // Update game time and difficulty
  m_gameTime += deltaTime;
  updateDifficulty(deltaTime);

  // Update combo timer
  if (m_comboTimer > 0.0f) {
    m_comboTimer -= deltaTime;
    if (m_comboTimer <= 0.0f) {
      // Combo expired
      m_comboMultiplier = 1.0f;
      m_comboTimer = 0.0f;
    }
  }

  // Build drift meter while drifting
  if (isDrifting && playerSpeed > 100.0f) {
    m_driftMeter = std::min(1.0f, m_driftMeter + deltaTime * 0.5f);

    // Award continuous drift points
    float speedBonus = playerSpeed / SPEED_BONUS_DIVISOR;
    float points = BASE_DRIFT_SCORE * driftAmount * speedBonus *
                   m_comboMultiplier * deltaTime;
    addScore(static_cast<std::uint64_t>(points));

    // Keep combo alive while drifting
    m_comboTimer = MAX_COMBO_TIME;
  } else {
    // Decay drift meter when not drifting
    m_driftMeter = std::max(0.0f, m_driftMeter - deltaTime * 0.3f);
  }
}

void ScoreManager::onDriftEnd(float driftDuration, float averageSpeed) {
  if (driftDuration < 0.5f)
    return; // Minimum drift time for bonus

  // Calculate drift bonus
  float durationBonus = std::min(5.0f, driftDuration); // Cap at 5 seconds
  float speedBonus = averageSpeed / SPEED_BONUS_DIVISOR;
  float baseBonus = 100.0f * durationBonus * speedBonus;

  // Apply combo multiplier
  std::uint64_t bonusPoints =
      static_cast<std::uint64_t>(baseBonus * m_comboMultiplier);
  addScore(bonusPoints);

  // Increase combo multiplier
  m_comboMultiplier = std::min(8.0f, m_comboMultiplier + 0.5f);
  m_comboTimer = MAX_COMBO_TIME;
}

void ScoreManager::onCollision() {
  // Reset combo on collision
  m_comboMultiplier = 1.0f;
  m_comboTimer = 0.0f;
  m_driftMeter = 0.0f;
}

void ScoreManager::addScore(std::uint64_t points) { m_score += points; }

void ScoreManager::updateDifficulty(float deltaTime) {
  // Slowly increase difficulty over time
  if (m_difficulty < MAX_DIFFICULTY) {
    m_difficulty += DIFFICULTY_INCREASE_RATE * deltaTime;
    m_difficulty = std::min(MAX_DIFFICULTY, m_difficulty);
  }
}
