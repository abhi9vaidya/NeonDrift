#include "graphics/ParticleSystem.hpp"
#include <cmath>
#include <random>

// Random number generator
static std::random_device rd;
static std::mt19937 rng(rd());
static std::uniform_real_distribution<float> randFloat(0.0f, 1.0f);
static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

ParticleSystem::ParticleSystem(std::size_t maxParticles)
    : m_particles(maxParticles), m_vertices(sf::PrimitiveType::Triangles) {
  // Initialize all particles as inactive
  for (auto &p : m_particles) {
    p.active = false;
  }
}

Particle *ParticleSystem::getAvailableParticle() {
  for (auto &p : m_particles) {
    if (!p.active) {
      return &p;
    }
  }
  return nullptr; // Pool exhausted
}

void ParticleSystem::update(float deltaTime) {
  for (auto &p : m_particles) {
    if (!p.active)
      continue;

    // Update lifetime
    p.lifetime -= deltaTime;
    if (p.lifetime <= 0.0f) {
      p.active = false;
      continue;
    }

    // Update position
    p.position += p.velocity * deltaTime;

    // Apply drag
    p.velocity *= 0.98f;

    // Fade out based on lifetime
    float lifeRatio = p.lifetime / p.maxLifetime;
    p.color.a = static_cast<std::uint8_t>(255 * lifeRatio);

    // Shrink slightly
    p.size *= (0.98f + 0.02f * lifeRatio);
  }
}

void ParticleSystem::render(sf::RenderWindow &window) {
  m_vertices.clear();

  for (const auto &p : m_particles) {
    if (!p.active)
      continue;

    // Create a quad (2 triangles) for each particle
    float halfSize = p.size * 0.5f;

    sf::Vertex v1, v2, v3, v4;
    v1.position = p.position + sf::Vector2f(-halfSize, -halfSize);
    v2.position = p.position + sf::Vector2f(halfSize, -halfSize);
    v3.position = p.position + sf::Vector2f(halfSize, halfSize);
    v4.position = p.position + sf::Vector2f(-halfSize, halfSize);

    v1.color = v2.color = v3.color = v4.color = p.color;

    // Triangle 1
    m_vertices.append(v1);
    m_vertices.append(v2);
    m_vertices.append(v3);

    // Triangle 2
    m_vertices.append(v1);
    m_vertices.append(v3);
    m_vertices.append(v4);
  }

  // Enable additive blending for glow effect
  sf::RenderStates states;
  states.blendMode = sf::BlendAdd;

  window.draw(m_vertices, states);
}

void ParticleSystem::emitDriftTrail(const sf::Vector2f &position,
                                    const sf::Vector2f &velocity,
                                    float driftAmount, float direction) {
  // Emit 2-4 particles per call
  int count = 2 + static_cast<int>(driftAmount * 2);

  for (int i = 0; i < count; ++i) {
    Particle *p = getAvailableParticle();
    if (!p)
      return;

    p->active = true;
    p->position = position;

    // Add some randomness to velocity
    float spreadAngle = (randFloat(rng) - 0.5f) * 30.0f;
    float speed = 20.0f + randFloat(rng) * 40.0f;
    float angle = std::atan2(velocity.y, velocity.x) + 3.14159f +
                  spreadAngle * 0.0174533f;

    p->velocity =
        sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

    // Neon colors based on drift amount
    // Blend from cyan to magenta as drift intensifies
    float blend = driftAmount * 0.8f + randFloat(rng) * 0.2f;
    std::uint8_t r = static_cast<std::uint8_t>(0 + 255 * blend);
    std::uint8_t g = static_cast<std::uint8_t>(255 * (1.0f - blend * 0.3f));
    std::uint8_t b = static_cast<std::uint8_t>(255);
    p->color = sf::Color(r, g, b, 200);

    p->lifetime = DRIFT_PARTICLE_LIFETIME * (0.7f + randFloat(rng) * 0.6f);
    p->maxLifetime = p->lifetime;
    p->size = BASE_SIZE * (1.0f + driftAmount * 0.5f);
  }
}

void ParticleSystem::emitCollisionBurst(const sf::Vector2f &position,
                                        const sf::Color &color) {
  // Emit 15-25 particles in all directions
  int count = 15 + static_cast<int>(randFloat(rng) * 10);

  for (int i = 0; i < count; ++i) {
    Particle *p = getAvailableParticle();
    if (!p)
      return;

    p->active = true;
    p->position = position;

    float angle = randAngle(rng) * 0.0174533f; // Convert to radians
    float speed = 150.0f + randFloat(rng) * 200.0f;

    p->velocity =
        sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

    // Orange-red sparks
    p->color =
        sf::Color(255, static_cast<std::uint8_t>(100 + randFloat(rng) * 100),
                  static_cast<std::uint8_t>(randFloat(rng) * 50), 255);

    p->lifetime = COLLISION_PARTICLE_LIFETIME * (0.5f + randFloat(rng) * 0.5f);
    p->maxLifetime = p->lifetime;
    p->size = BASE_SIZE * (0.8f + randFloat(rng) * 0.6f);
  }
}

void ParticleSystem::emitSpeedLines(const sf::Vector2f &position, float speed,
                                    float rotation) {
  if (speed < 200.0f)
    return; // Only at high speeds

  // Emit based on speed
  float intensity = (speed - 200.0f) / 400.0f;
  if (randFloat(rng) > intensity)
    return;

  Particle *p = getAvailableParticle();
  if (!p)
    return;

  p->active = true;

  // Spawn slightly behind player
  float rad = rotation * 0.0174533f;
  p->position =
      position - sf::Vector2f(std::cos(rad) * 30.0f, std::sin(rad) * 30.0f);

  // Move opposite to player direction
  p->velocity = sf::Vector2f(-std::cos(rad) * speed * 0.3f,
                             -std::sin(rad) * speed * 0.3f);

  // White/cyan speed lines
  p->color = sf::Color(200, 255, 255, 150);

  p->lifetime = 0.15f;
  p->maxLifetime = p->lifetime;
  p->size = BASE_SIZE * 0.5f;
}

void ParticleSystem::clear() {
  for (auto &p : m_particles) {
    p.active = false;
  }
}
