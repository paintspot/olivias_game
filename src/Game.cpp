#include "Game.hpp"

void Game::run() {
  sf::Clock clock;
  // Use a local variable to track vertical position
  float verticalPos = m_character.getPosition().y;
  while (m_window.isOpen()) {
    sf::Event event;
    while (m_window.pollEvent(event)) {
      // Check if frog is on ground or platform
      bool canJump = false;
      sf::Vector2f pos = m_character.getPosition();
      float frogHeight = m_character.getBounds().height;
      constexpr float epsilon = 2.0f; // Allow small margin for float errors
      for (const auto &platform : m_platforms) {
        float platformTop = platform.getBounds().top;
        if (
          std::abs((pos.y + frogHeight) - platformTop) < epsilon &&
          m_verticalVelocity == 0
        ) {
          canJump = true;
          break;
        }
      }
      float groundY = 1000.0f;
      if (std::abs((pos.y + frogHeight) - groundY) < epsilon && m_verticalVelocity == 0) canJump = true;
      m_event_handler.handleEvents(event, m_sprite_pos, canJump, m_verticalVelocity);
    }

    float dt = clock.restart().asSeconds();
    // Only update horizontal position from m_sprite_pos, keep vertical from velocity
    sf::Vector2f currentPos = m_character.getPosition();
    sf::Vector2f newPos(m_sprite_pos.getValue().x, currentPos.y);
    m_character.setPosition(newPos);
    update(dt);
    render();
  }
}

void Game::render() {
  m_window.clear();

  // Draw platforms
  for (auto &platform : m_platforms) {
    platform.draw(m_window);
  }

  m_character.draw(m_window);
  m_window.display();
}

void Game::update(float dt) {
  m_verticalVelocity += GRAVITY * dt;
  sf::FloatRect frogBounds = m_character.getBounds();
  float prevFeet = frogBounds.top + frogBounds.height;

  // Move vertically
  sf::Vector2f position = m_character.getPosition();
  position.y += m_verticalVelocity * dt;
  m_character.setPosition(position);
  frogBounds = m_character.getBounds(); // update after move
  float newFeet = frogBounds.top + frogBounds.height;

  // Find the highest platform the frog lands on (if any)
  float bestPlatformY = -1;
  for (const auto &platform : m_platforms) {
    sf::FloatRect plat = platform.getBounds();
    bool horizontalOverlap = frogBounds.left + frogBounds.width > plat.left && frogBounds.left < plat.left + plat.width;
    // If frog is moving down and feet cross the platform top
    if (horizontalOverlap && m_verticalVelocity > 0 && prevFeet <= plat.top && newFeet >= plat.top) {
      if (bestPlatformY < 0 || plat.top < bestPlatformY) {
        bestPlatformY = plat.top;
      }
    }
  }
  if (bestPlatformY >= 0) {
    // Snap so frog's feet are exactly on the platform
    position.y += (bestPlatformY - newFeet);
    m_verticalVelocity = 0;
    m_character.setPosition(position);
    frogBounds = m_character.getBounds();
    newFeet = frogBounds.top + frogBounds.height;
  }

  // Ground collision
  float groundY = 2000.0f;
  if (newFeet > groundY) {
    position.y += (groundY - newFeet);
    m_verticalVelocity = 0;
    m_character.setPosition(position);
  }

  m_character.update(dt);
}