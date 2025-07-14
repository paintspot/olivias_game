#pragma once
#include <SFML/Graphics.hpp>
#include "interpolated.hpp"
#include "functions.hpp"

#define START_POS {200.0f, 200.0f}

class EventHandler {
public:
  EventHandler(sf::RenderWindow &window, sf::RectangleShape &rectangle)
      : m_window(window), m_rectangle(rectangle) {}

  void handleEvents(sf::Event event, Interpolated<sf::Vector2f> &rectangle_pos) {
    sf::Vector2f new_pos = m_rectangle.getPosition();
    switch (event.type) {
    case sf::Event::KeyPressed:
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        rectangle_pos = {new_pos.x - 1000, new_pos.y};
        rectangle_pos.setDuration(1.0f);
        rectangle_pos.transition = TransitionFunction::EaseOutElastic;
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        rectangle_pos = {new_pos.x + 1000, new_pos.y};
        rectangle_pos.setDuration(1.0f);
        rectangle_pos.transition = TransitionFunction::EaseOutElastic;
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
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
  sf::RectangleShape &m_rectangle;
};

class Game {
public:
  Game(sf::Vector2u window_size, std::string const &title, uint32_t style)
      : m_window(sf::VideoMode({window_size.x, window_size.y}), title, style),
        m_window_size(window_size), 
        m_rectangle(START_POS),
        m_rectangle_pos(START_POS),
        m_event_handler(m_window, m_rectangle) {
    m_rectangle.setFillColor(sf::Color::Red);
    m_rectangle.setPosition(200, 200);
  }

  void run() {
    while (m_window.isOpen()) {
      sf::Event event;
      while (m_window.pollEvent(event)) {
        m_event_handler.handleEvents(event, m_rectangle_pos);
      }
      render();
    }
  }

  void render() {
    m_rectangle.setPosition(m_rectangle_pos);
    m_window.clear();
    m_window.draw(m_rectangle);
    m_window.display();
  }

private:
  sf::RenderWindow m_window;
  sf::Vector2u m_window_size;
  sf::RectangleShape m_rectangle;
  Interpolated<sf::Vector2f> m_rectangle_pos;
  EventHandler m_event_handler;
};