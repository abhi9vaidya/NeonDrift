#pragma once

#include <cstdint>

/**
 * Manages scoring, combo system, and difficulty progression
 */
class ScoreManager {
public:
  ScoreManager();

  // Update scoring (call each frame while playing)
  void update(float deltaTime, float playerSpeed, bool isDrifting,
              float driftAmount);

  // Score events
  void onDriftEnd(float driftDuration, float averageSpeed);
  void onCollision();

  // Getters
  std::uint64_t getScore() const { return m_score; }
  float getComboMultiplier() const { return m_comboMultiplier; }
  float getComboTimer() const { return m_comboTimer; }
  float getMaxComboTimer() const { return MAX_COMBO_TIME; }
  float getDifficulty() const { return m_difficulty; }
  float getDriftMeter() const { return m_driftMeter; }
  bool isComboActive() const { return m_comboTimer > 0.0f; }

  // Reset for new game
  void reset();

private:
  void addScore(std::uint64_t points);
  void updateDifficulty(float deltaTime);

  std::uint64_t m_score;
  float m_comboMultiplier;
  float m_comboTimer;
  float m_driftMeter; // Fills up while drifting
  float m_difficulty; // 0.0 to 2.0, increases over time
  float m_gameTime;

  // Scoring constants
  static constexpr float MAX_COMBO_TIME = 3.0f;
  static constexpr float BASE_DRIFT_SCORE = 10.0f;
  static constexpr float SPEED_BONUS_DIVISOR = 100.0f;
  static constexpr float DIFFICULTY_INCREASE_RATE = 0.01f;
  static constexpr float MAX_DIFFICULTY = 2.0f;
};
