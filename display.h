#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include <list>
#include <string>
#include <random>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include "TetrisPiece.h"

class Display {
private:
    const float width, height;
    const std::string title;
    uint32_t frame;
    std::vector<std::vector<sf::RectangleShape>> grid;
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;
    std::vector<Piece> pieces;
    TetrisPiece piece;
    TetrisPiece previewPiece;
    std::vector<std::vector<sf::RectangleShape>> previewGrid;
    uint32_t linesCleared;
    uint64_t score;
    sf::Font font;
    sf::Text scoreText;
    sf::Color blankColor;
    std::vector<sf::RectangleShape> effects;

public:
    Display() = delete;
    Display(float width, float height, const std::string& title);

    void render_loop();

private:
    void draw_grid(sf::RenderWindow& window);
    void draw_preview_grid(sf::RenderWindow& window);
    void draw_text(sf::RenderWindow& window);
    void draw_effects(sf::RenderWindow& window);
    bool is_closing(const sf::Event& event);
    void handle_event(const sf::Event& event);
    void update_frame();
    void update_score();
    void update_effects();
    void clean_up_effects();
    bool game_over() const;

    void shift_grid_down_to(uint32_t lineNumber);
    Piece generate_random_tetris_piece();

    bool piece_can_move_down();
    bool piece_can_move_left();
    bool piece_can_move_right();
    bool piece_can_rotate();
};

#endif /* DRAW_H */
