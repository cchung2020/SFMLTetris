#ifndef PIECE_H
#define PIECE_H

#include <set>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

enum class Piece {
    T,
    O,
    L,
    FlippedL,
    S,
    FlippedS,
    Line
};

enum class Direction {
    Up,
    Left,
    Right,
    Down
};

struct TetrisPiece {
    std::vector<sf::Vector2u> coords;
    std::vector<sf::Vector2u> shape;
    sf::Vector2u origin;
    sf::Color color;
    Piece type;
    Direction face;
    
    TetrisPiece(Piece type);
    void move_down();
    void move_left();
    void move_right();
    void rotate();

    std::vector<sf::Vector2u> rotated_shape();
};

#endif /* PIECE_H */
