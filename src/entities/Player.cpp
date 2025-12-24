#include "entities/Player.hpp"
#include <cmath>
#include <cstdint>

// Helper to convert degrees to radians
constexpr float DEG_TO_RAD = 3.14159265f / 180.0f;

Player::Player()
    : m_position(640.0f, 400.0f) // Center of screen
      ,
      m_velocity(0.0f, 0.0f), m_rotation(-90.0f) // Facing up
      ,
      m_angularVelocity(0.0f), m_isDrifting(false), m_driftAmount(0.0f),
      m_driftDirection(0.0f), m_baseColor(0, 255, 255) // Cyan
      ,
      m_glowColor(255, 0, 255) // Magenta
{
  // Create arrow/car shaped polygon
  m_shape.setPointCount(5);
  m_shape.setPoint(0, sf::Vector2f(30.0f, 0.0f));    // Front tip
  m_shape.setPoint(1, sf::Vector2f(-15.0f, -18.0f)); // Back left outer
  m_shape.setPoint(2, sf::Vector2f(-8.0f, 0.0f));    // Back center indent
  m_shape.setPoint(3, sf::Vector2f(-15.0f, 18.0f));  // Back right outer
  m_shape.setPoint(4, sf::Vector2f(30.0f, 0.0f));    // Close to front

  m_shape.setFillColor(m_baseColor);
  m_shape.setOutlineColor(sf::Color::White);
  m_shape.setOutlineThickness(2.0f);
  m_shape.setOrigin(sf::Vector2f(0.0f, 0.0f));
}

void Player::reset() {
  m_position = sf::Vector2f(640.0f, 400.0f);
  m_velocity = sf::Vector2f(0.0f, 0.0f);
  m_rotation = -90.0f;
  m_angularVelocity = 0.0f;
  m_isDrifting = false;
  m_driftAmount = 0.0f;
  m_driftDirection = 0.0f;
}

float Player::getSpeed() const {
  return std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
}

void Player::update(float deltaTime, const InputManager &input) {
  applyInput(input, deltaTime);
  applyPhysics(deltaTime);
  updateVisuals();
}

void Player::applyInput(const InputManager &input, float deltaTime) {
  float speed = getSpeed();

  // Get forward direction from rotation
  float radians = m_rotation * DEG_TO_RAD;
  sf::Vector2f forward(std::cos(radians), std::sin(radians));

  // Acceleration
  if (input.isAccelerating()) {
    m_velocity += forward * ACCELERATION * deltaTime;
  }

  // Braking
  if (input.isBraking()) {
    // If moving forward, slow down
    // If stopped, reverse slowly
    float forwardDot = m_velocity.x * forward.x + m_velocity.y * forward.y;

    if (forwardDot > 10.0f) {
      m_velocity -= forward * BRAKE_FORCE * deltaTime;
    } else {
      // Reverse (slower)
      m_velocity -= forward * (ACCELERATION * 0.4f) * deltaTime;
    }
  }

  // Steering (only when moving)
  if (speed > MIN_SPEED_TO_TURN) {
    float turnMultiplier = m_isDrifting ? DRIFT_TURN_MULTIPLIER : 1.0f;

    if (input.isTurningLeft()) {
      m_rotation -= TURN_SPEED * turnMultiplier * deltaTime;
    }
    if (input.isTurningRight()) {
      m_rotation += TURN_SPEED * turnMultiplier * deltaTime;
    }

    // Track drift direction
    if (input.isTurningLeft())
      m_driftDirection = -1.0f;
    else if (input.isTurningRight())
      m_driftDirection = 1.0f;
  }

  // Keep rotation in bounds
  while (m_rotation > 360.0f)
    m_rotation -= 360.0f;
  while (m_rotation < 0.0f)
    m_rotation += 360.0f;

  // Drift activation
  bool wantsToDrift = input.isDrifting() && speed > MIN_SPEED_TO_TURN * 2;

  if (wantsToDrift && !m_isDrifting) {
    // Start drifting
    m_isDrifting = true;
    m_driftAmount = 0.0f;
  } else if (!wantsToDrift && m_isDrifting) {
    // End drift
    m_isDrifting = false;
    m_driftAmount = 0.0f;
  }

  // Build up drift amount while drifting
  if (m_isDrifting) {
    m_driftAmount = std::min(1.0f, m_driftAmount + deltaTime * 2.0f);
  }
}

void Player::applyPhysics(float deltaTime) {
  // Speed limit
  float speed = getSpeed();
  if (speed > MAX_SPEED) {
    float scale = MAX_SPEED / speed;
    m_velocity.x *= scale;
    m_velocity.y *= scale;
  }

  // Get forward and right directions
  float radians = m_rotation * DEG_TO_RAD;
  sf::Vector2f forward(std::cos(radians), std::sin(radians));
  sf::Vector2f right(-std::sin(radians), std::cos(radians));

  // Decompose velocity into forward and lateral components
  float forwardSpeed = m_velocity.x * forward.x + m_velocity.y * forward.y;
  float lateralSpeed = m_velocity.x * right.x + m_velocity.y * right.y;

  // Apply different friction based on drift state
  float friction = m_isDrifting ? DRIFT_FRICTION : FRICTION;
  float lateralFriction = m_isDrifting ? DRIFT_SLIDE_FACTOR : 0.9f;

  // Apply friction
  forwardSpeed *= friction;
  lateralSpeed *= lateralFriction;

  // Reconstruct velocity
  m_velocity = forward * forwardSpeed + right * lateralSpeed;

  // Update position
  m_position += m_velocity * deltaTime;

  // Very low speed = stop completely (prevent jittering)
  if (getSpeed() < 5.0f && !m_isDrifting) {
    m_velocity = sf::Vector2f(0.0f, 0.0f);
  }
}

void Player::updateVisuals() {
  m_shape.setPosition(m_position);
  m_shape.setRotation(sf::degrees(m_rotation));

  // Color shift based on drift and speed
  if (m_isDrifting) {
    // Blend towards magenta when drifting
    float blend = m_driftAmount;
    std::uint8_t r = static_cast<std::uint8_t>(m_baseColor.r * (1.0f - blend) +
                                               m_glowColor.r * blend);
    std::uint8_t g = static_cast<std::uint8_t>(m_baseColor.g * (1.0f - blend) +
                                               m_glowColor.g * blend);
    std::uint8_t b = static_cast<std::uint8_t>(m_baseColor.b * (1.0f - blend) +
                                               m_glowColor.b * blend);
    m_shape.setFillColor(sf::Color(r, g, b));
  } else {
    // Speed-based color intensity
    float speedRatio = getSpeed() / MAX_SPEED;
    std::uint8_t intensity = static_cast<std::uint8_t>(180 + 75 * speedRatio);
    m_shape.setFillColor(sf::Color(0, intensity, intensity));
  }
}

void Player::render(sf::RenderWindow &window) { window.draw(m_shape); }
