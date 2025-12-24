#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>


/**
 * Individual particle with position, velocity, and lifetime
 */
struct Particle {
  sf::Vector2f position;
  sf::Vector2f velocity;
  sf::Color color;
  float lifetime;    // Remaining time
  float maxLifetime; // Original lifetime for alpha calculation
  float size;
  bool active;
};

/**
 * Particle System for visual effects
 * Uses object pooling for efficiency
 */
class ParticleSystem {
public:
  ParticleSystem(std::size_t maxParticles = 2000);

  // Update all particles
  void update(float deltaTime);

  // Render all active particles
  void render(sf::RenderWindow &window);

  // Spawn particles
  void emitDriftTrail(const sf::Vector2f &position,
                      const sf::Vector2f &velocity, float driftAmount,
                      float direction);
  void emitCollisionBurst(const sf::Vector2f &position, const sf::Color &color);
  void emitSpeedLines(const sf::Vector2f &position, float speed,
                      float rotation);

  // Clear all particles
  void clear();

private:
  // Get next available particle from pool
  Particle *getAvailableParticle();

  std::vector<Particle> m_particles;
  sf::VertexArray m_vertices;

  // Particle appearance
  static constexpr float BASE_SIZE = 4.0f;
  static constexpr float DRIFT_PARTICLE_LIFETIME = 0.6f;
  static constexpr float COLLISION_PARTICLE_LIFETIME = 0.4f;
};
