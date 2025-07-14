#pragma once
#include <SFML/Graphics.hpp>

class EventHandler {
public:
  EventHandler(sf::RenderWindow &window, sf::RectangleShape &rectangle) : m_window(window), m_rectangle(rectangle) {}

  void handleEvents(sf::Event event) {
    sf::Vector2f pos = m_rectangle.getPosition();
    switch (event.type) {
    case sf::Event::KeyPressed:
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        m_rectangle.setPosition(pos.x-100, pos.y);
      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        m_rectangle.setPosition(pos.x+100, pos.y);
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
        m_rectangle({200,200}), 
        m_event_handler(m_window, m_rectangle) {
          m_rectangle.setFillColor(sf::Color::Red);
          m_rectangle.setPosition(200, 200);
        }

  void run() {
    while (m_window.isOpen()) {
      sf::Event event;
      while (m_window.pollEvent(event)) {
        m_event_handler.handleEvents(event);
      }
      render();
    }
  }

  void render() {
    m_window.clear();
    m_window.draw(m_rectangle);
    m_window.display();
  }

private:
  sf::RenderWindow m_window;
  sf::Vector2u m_window_size;
  sf::RectangleShape m_rectangle;
  EventHandler m_event_handler;
};