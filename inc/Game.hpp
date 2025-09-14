#pragma once
#include "functions.hpp"
#include "interpolated.hpp"
#include <SFML/Graphics.hpp>

#define JUMP_LENGTH 1.3f
#define FROG_SCALE 5.0f
#define START_POS {200.0f, 800.0f}
#define JUMP_VELOCITY -600.0f

struct AnimationFrame
{
  sf::IntRect rect;
  float duration; // time this frame is shown (in seconds)
};

class Animation
{
public:
  void addFrame(sf::IntRect rect, float duration)
  {
    m_frames.push_back({rect, duration});
  }

  void update(float dt)
  {
    if (m_frames.empty())
      return;
    m_elapsed += dt;
    while (m_elapsed >= m_frames[m_index].duration)
    {
      m_elapsed -= m_frames[m_index].duration;
      m_index = (m_index + 1) % m_frames.size();
    }
  }

  const sf::IntRect &getCurrentFrame() const { return m_frames[m_index].rect; }

  void reset()
  {
    m_index = 0;
    m_elapsed = 0.f;
  }
  void setDuration(float animation_duration)
  {
    float frame_duration = animation_duration / m_frames.size();
    for (auto &frame : m_frames)
    {
      frame.duration = frame_duration;
    }
  }

private:
  std::vector<AnimationFrame> m_frames;
  size_t m_index = 0;
  float m_elapsed = 0.f;
};

enum class Direction
{
  Left,
  Right
};

class Character
{
public:
  Character(const sf::Texture &texture, sf::Vector2f position)
      : m_direction(Direction::Right)
  {
    m_sprite.setTexture(texture);
    m_sprite.setPosition(position);
    m_sprite.setScale(FROG_SCALE, FROG_SCALE); // Default (facing right)
  }

  void setupAnimations()
  {
    for (int i = 0; i < 7; i++)
    {
      m_idleAnimation.addFrame({48 * i, 0, 48, 48}, 0.1f);
      m_hopAnimation.addFrame({48 * i, 48, 48, 48}, 0.1f);
    }
    m_currentAnimation = &m_idleAnimation;
  }

  void update(float dt)
  {
    if (m_isHopping)
    {
      m_hopTimer += dt;
      if (m_hopTimer >= m_hopDuration)
      {
        playIdle();
        m_isHopping = false;
      }
    }

    m_currentAnimation->update(dt);
    m_sprite.setTextureRect(m_currentAnimation->getCurrentFrame());
  }

  void setPosition(sf::Vector2f pos)
  {
    m_sprite.setPosition(pos);
  }

  sf::Vector2f getPosition() const
  {
    return m_sprite.getPosition();
  }

  void draw(sf::RenderWindow &window)
  {
    window.draw(m_sprite);
  }

  void face(Direction dir)
  {
    if (m_direction != dir)
    {
      m_direction = dir;
      if (dir == Direction::Left)
      {
        m_sprite.getScale();
        m_sprite.setScale(-FROG_SCALE, FROG_SCALE);
        m_sprite.setOrigin(48, 0); // Flip around vertical center
      }
      else
      {
        m_sprite.setScale(FROG_SCALE, FROG_SCALE);
        m_sprite.setOrigin(0, 0);
      }
    }
  }

  void playIdle()
  {
    if (m_currentAnimation != &m_idleAnimation)
    {
      m_currentAnimation = &m_idleAnimation;
      m_currentAnimation->reset();
    }
  }

  void playHop(float duration)
  {
    m_hopAnimation.setDuration(duration);
    m_hopDuration = duration;
    m_hopTimer = 0.f;
    m_isHopping = true;

    if (m_currentAnimation != &m_hopAnimation)
    {
      m_currentAnimation = &m_hopAnimation;
      m_currentAnimation->reset();
    }
  }

  sf::FloatRect getBounds() const
  {
    return m_sprite.getGlobalBounds();
  }

private:
  sf::Sprite m_sprite;
  Animation m_idleAnimation;
  Animation m_hopAnimation;
  Animation *m_currentAnimation = nullptr;
  Direction m_direction;
  bool m_isHopping = false;
  float m_hopTimer = 0.f;
  float m_hopDuration = 0.f;
};

class EventHandler
{
public:
  EventHandler(sf::RenderWindow &window, Character &character)
      : m_window(window), m_character(character) {}

  void handleEvents(sf::Event event, Interpolated<sf::Vector2f> &pos, bool canJump, float &verticalVelocity)
  {
    sf::Vector2f new_pos = m_character.getPosition();

    switch (event.type)
    {
    case sf::Event::KeyPressed:
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
      {
        pos = {new_pos.x - 100, new_pos.y};
        pos.setDuration(JUMP_LENGTH);
        pos.transition = TransitionFunction::EaseInOutElastic;
        m_character.face(Direction::Left);
        m_character.playHop(JUMP_LENGTH);
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
      {
        pos = {new_pos.x + 100, new_pos.y};
        pos.setDuration(JUMP_LENGTH);
        pos.transition = TransitionFunction::EaseInOutElastic;
        m_character.face(Direction::Right);
        m_character.playHop(JUMP_LENGTH);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump)
      {
        verticalVelocity = JUMP_VELOCITY;
        m_character.playHop(JUMP_LENGTH);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      {
        m_window.close();
      }
      break;

    case sf::Event::Closed:
      m_window.close();
      break;

    default:
      break;
    }
  }

private:
  sf::RenderWindow &m_window;
  Character &m_character;
};

class Platform
{
public:
  Platform(sf::Vector2f position, sf::Vector2f size)
      : m_position(position), m_size(size)
  {
    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color::Green);
  }

  void draw(sf::RenderWindow &window) { window.draw(m_shape); }

  sf::FloatRect getBounds() const { return m_shape.getGlobalBounds(); }

private:
  sf::Vector2f m_position;
  sf::Vector2f m_size;
  sf::RectangleShape m_shape;
};

class Game
{
public:
  Game(sf::Vector2u window_size, std::string const &title, uint32_t style)
      : m_window(sf::VideoMode({window_size.x, window_size.y}), title, style),
        m_window_size(window_size),
        m_character(m_texture, START_POS),
        m_sprite_pos(START_POS),
        m_event_handler(m_window, m_character)
  {
    if (!m_texture.loadFromFile("./assets/images/Frog_Sheet.png"))
    {
      throw std::runtime_error("Failed to load spritesheet");
    }

    m_character.setupAnimations();

    // Initialize platforms
    m_platforms.emplace_back(sf::Vector2f(0, 1060), sf::Vector2f(1920, 20));
    //m_platforms.emplace_back(sf::Vector2f(0, 800), sf::Vector2f(1920, 20));
  }
  void run();
  void render();
  void update(float dt);

private:
  sf::RenderWindow m_window;
  sf::Vector2u m_window_size;
  sf::Texture m_texture;
  Character m_character;
  Interpolated<sf::Vector2f> m_sprite_pos;
  EventHandler m_event_handler;
  std::vector<Platform> m_platforms;

  float m_verticalVelocity = 0.0f; // Gravity velocity
  const float GRAVITY = 1200.0f;

};