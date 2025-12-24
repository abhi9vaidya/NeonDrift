#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <unordered_set>

/**
 * Centralized input handling
 * Tracks currently pressed keys for smooth gameplay
 */
class InputManager {
public:
  // Update key states based on events
  void keyPressed(sf::Keyboard::Key key) { m_pressedKeys.insert(key); }

  void keyReleased(sf::Keyboard::Key key) { m_pressedKeys.erase(key); }

  // Check if a key is currently held
  bool isKeyHeld(sf::Keyboard::Key key) const {
    return m_pressedKeys.count(key) > 0;
  }

  // Convenience methods for game controls
  bool isAccelerating() const {
    return isKeyHeld(sf::Keyboard::Key::W) || isKeyHeld(sf::Keyboard::Key::Up);
  }

  bool isBraking() const {
    return isKeyHeld(sf::Keyboard::Key::S) ||
           isKeyHeld(sf::Keyboard::Key::Down);
  }

  bool isTurningLeft() const {
    return isKeyHeld(sf::Keyboard::Key::A) ||
           isKeyHeld(sf::Keyboard::Key::Left);
  }

  bool isTurningRight() const {
    return isKeyHeld(sf::Keyboard::Key::D) ||
           isKeyHeld(sf::Keyboard::Key::Right);
  }

  bool isDrifting() const { return isKeyHeld(sf::Keyboard::Key::Space); }

  void clear() { m_pressedKeys.clear(); }

private:
  std::unordered_set<sf::Keyboard::Key> m_pressedKeys;
};
