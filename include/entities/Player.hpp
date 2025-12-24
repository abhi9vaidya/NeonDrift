#pragma once

#include "core/InputManager.hpp"
#include <SFML/Graphics.hpp>


/**
 * Player vehicle with physics-based movement
 * Features: acceleration, friction, rotation steering, drift
 */
class Player {
public:
  Player();

  // Core update
  void update(float deltaTime, const InputManager &input);
  void render(sf::RenderWindow &window);

  // Getters
  sf::Vector2f getPosition() const { return m_position; }
  sf::Vector2f getVelocity() const { return m_velocity; }
  float getSpeed() const;
  float getRotation() const { return m_rotation; }
  bool isDrifting() const { return m_isDrifting; }
  float getDriftAmount() const { return m_driftAmount; }

  // Position control
  void setPosition(const sf::Vector2f &pos) { m_position = pos; }
  void reset();

private:
  // Physics calculations
  void applyInput(const InputManager &input, float deltaTime);
  void applyPhysics(float deltaTime);
  void updateVisuals();

  // Position & movement
  sf::Vector2f m_position;
  sf::Vector2f m_velocity;
  float m_rotation; // degrees
  float m_angularVelocity;

  // Drift state
  bool m_isDrifting;
  float m_driftAmount;    // 0.0 to 1.0, how much we're sliding
  float m_driftDirection; // -1 left, 1 right, 0 none

  // Visual representation
  sf::ConvexShape m_shape;
  sf::Color m_baseColor;
  sf::Color m_glowColor;

  // Physics constants
  static constexpr float MAX_SPEED = 600.0f;
  static constexpr float ACCELERATION = 800.0f;
  static constexpr float BRAKE_FORCE = 600.0f;
  static constexpr float FRICTION = 0.98f;       // Normal friction
  static constexpr float DRIFT_FRICTION = 0.92f; // Less grip when drifting
  static constexpr float TURN_SPEED = 180.0f;    // Degrees per second
  static constexpr float DRIFT_TURN_MULTIPLIER = 1.5f;
  static constexpr float DRIFT_SLIDE_FACTOR = 0.85f;
  static constexpr float MIN_SPEED_TO_TURN = 50.0f;
};
