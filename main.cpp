#include "Game.hpp"

int main() {
  sf::Vector2u window_size{1920, 1080};
  
  Game game(window_size, "Olivias game", sf::Style::Default);
  game.run();

  return 0;
}