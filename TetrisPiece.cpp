#include <set>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "TetrisPiece.h"

TetrisPiece::TetrisPiece(Piece type) : 
        origin({4,4}), 
        type(type),
        face(Direction::Down) {
    switch(type) {
        case Piece::T: {
            origin.x -= 1;
            shape = { {1,0}, {1,1}, {1,2}, {2,1} };
            color = sf::Color::Magenta;
            break;
        }
        case Piece::O: {
            shape = { {0,0}, {0,1}, {1,0}, {1,1} };
            color = sf::Color::Yellow;
            break;
        }
        case Piece::L: {
            shape = { {0,0}, {1,0}, {2,0}, {2,1} };
            color = sf::Color::Blue;
            break;
        }
        case Piece::FlippedL: {
            shape = { {0,1}, {1,1}, {2,1}, {2,0} };
            color = sf::Color(255, 165, 0); /* orange*/
            break;
        }
        case Piece::S: {
            shape = { {0,1}, {0,2}, {1,0}, {1,1} };
            color = sf::Color::Green;
            break;
        }
        case Piece::FlippedS: {
            shape = { {0,0}, {0,1}, {1,1}, {1,2} };
            color = sf::Color::Red;
            break;
        }
        case Piece::Line: {
            shape = { {0,0}, {1,0}, {2,0}, {3,0} };
            color = sf::Color::Cyan;
            break;
        }
    }

    coords = shape;
    for (auto& [x, y] : coords) {
        x += origin.x;
        y += origin.y;
    }
}

void TetrisPiece::move_down() {
    origin.x++;
    for (auto& [x, _y] : coords) {
        x++;
    }
}

void TetrisPiece::move_left() {
    origin.y--;
    for (auto& [_x, y] : coords) {
        y--;
    }
}

void TetrisPiece::move_right() {
    origin.y++;
    for (auto& [_x, y] : coords) {
        y++;
    }
}

void TetrisPiece::rotate() {
    shape = rotated_shape();
    coords = shape;
    for (auto& [x, y] : coords) {
        x += origin.x;
        y += origin.y;
    }

    switch(type) {
        case Piece::S:
        case Piece::FlippedS: 
        case Piece::Line: {
            if (face != Direction::Down) {
                face = Direction::Right;
            }
        }
        default: {}
    }

    if (face == Direction::Down) {
        face = Direction::Left;
    }
    else if (face == Direction::Left) {
        face = Direction::Up;
    }
    else if (face == Direction::Up) {
        face = Direction::Right;
    }
    else if (face == Direction::Right) {
        face = Direction::Down;
    }
}

std::vector<sf::Vector2u> TetrisPiece::rotated_shape() {
    switch(type) {
        case Piece::T: {
            if (face == Direction::Down) {
                return { {1,0}, {1,1}, {0,1}, {2,1} };
            }
            else if (face == Direction::Left) {
                return { {1,0}, {1,1}, {0,1}, {1,2} };
            }
            else if (face == Direction::Up) {
                return { {0,1}, {1,1}, {2,1}, {1,2} };
            }
            else if (face == Direction::Right) {
                return { {1,0}, {1,1}, {1,2}, {2,1} };
            }
        }
        case Piece::O: {
            return shape;
        }
        case Piece::L: {
            if (face == Direction::Down) {
                return { {1,0}, {1,1}, {1,2}, {2,0} };
            }
            else if (face == Direction::Left) {
                return { {0,1}, {1,1}, {2,1}, {0,0} };
            }
            else if (face == Direction::Up) {
                return { {1,0}, {1,1}, {1,2}, {0,2} };
            }
            else if (face == Direction::Right) {
                return { {0,0}, {1,0}, {2,0}, {2,1} };
            }
        }
        case Piece::FlippedL: {
            if (face == Direction::Down) {
                return { {1,0}, {1,1}, {1,2}, {0,0} };
            }
            else if (face == Direction::Left) {
                return { {0,1}, {1,1}, {2,1}, {0,2} };
            }
            else if (face == Direction::Up) {
                return { {1,0}, {1,1}, {1,2}, {2,2} };
            }
            else if (face == Direction::Right) {
                return { {0,1}, {1,1}, {2,1}, {2,0} };
            }
        }
        case Piece::S: {
            if (face == Direction::Down) {
                return { {0,0}, {1,0}, {1,1}, {2,1} };
            }
            else {
                return { {0,1}, {0,2}, {1,0}, {1,1} };
            }
        }
        case Piece::FlippedS: {
            if (face == Direction::Down) {
                return { {1,0}, {2,0}, {0,1}, {1,1} };
            }
            else {
                return { {0,0}, {0,1}, {1,1}, {1,2} };
            }
        }
        case Piece::Line:
        default: {
            if (face == Direction::Down) {
                return { {1,0}, {1,1}, {1,2}, {1,3} };
            }
            else {
                return { {0,0}, {1,0}, {2,0}, {3,0} };
            }
        }
    }
}

