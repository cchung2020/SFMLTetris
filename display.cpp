#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include <algorithm>
#include "display.h"

Display::Display(float width, float height, const std::string& title) : 
        width(width),
        height(height),
        title(title),
        frame(1),
        grid(24, std::vector<sf::RectangleShape>(10)),
        generator(std::chrono::system_clock::now().time_since_epoch().count()),
        distribution(0, 6), 
        piece(generate_random_tetris_piece()),
        previewPiece(generate_random_tetris_piece()),
        previewGrid(4, std::vector<sf::RectangleShape>(4)),
        linesCleared(0),
        score(0),
        blankColor(sf::Color::White) {
    for (int i = 0; i < 24; i++) {
        const float squareLength = std::min(width, height)/20;
        
        for (int j = 0; j < 10; j++) {
            sf::RectangleShape shape({squareLength/2, squareLength/2});
            shape.setFillColor(blankColor);
            shape.move({squareLength*j + width/4, squareLength*i});

            grid[i][j] = shape;
        }
    }

    for (int i = 0; i < 4; i++) {
        const float squareLength = std::min(width, height)/20;
        
        for (int j = 0; j < 4; j++) {
            sf::RectangleShape shape({squareLength/2, squareLength/2});
            shape.setFillColor(sf::Color::Black);
            shape.move({squareLength*(j+1) + 0.75f*width, squareLength*(i+4)});

            previewGrid[i][j] = shape;
        }
    }

    if (!font.loadFromFile("assets/BlackOpsOne-Regular.ttf")) {
        std::cerr << std::string("Put the font in the assets folder!\n");
        exit(1);
    }
    
    scoreText.setFont(font);
    scoreText.setCharacterSize(width/12);
    scoreText.setFillColor(sf::Color::Red);
}

void Display::render_loop() {
    sf::RenderWindow window(sf::VideoMode(width, height), title);
    window.setFramerateLimit(60);

    /*
    sf::Music music;
    if (!music.openFromFile("assets/videoplayback.ogg")) {
        std::cerr << std::string("Put the music in the assets folder!\n");
        exit(1);
    }
    music.play();
    */

   
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (is_closing(event)) {
                window.close();
            }
            else {
                handle_event(event);
            }
        }

        window.clear();
        if (!game_over()) {
            update_frame();
            update_score();
        }
        update_effects();
        draw_grid(window);
        draw_preview_grid(window);
        draw_text(window);
        draw_effects(window);
        clean_up_effects();
        window.display();
    }
}

void Display::draw_grid(sf::RenderWindow& window) {
    bool gameOver = game_over();
    // drawing can temporarily affects the game_over status

    for (auto& [x, y] : piece.coords) {
        grid[x][y].setFillColor(piece.color);
    }
    for (size_t i = 4; i < grid.size(); i++) {
        for (const auto& square : grid[i]) {
            window.draw(square);
        }
    }
    if (!gameOver) for (auto& [x, y] : piece.coords) {
        grid[x][y].setFillColor(blankColor);
    }
}

void Display::draw_preview_grid(sf::RenderWindow& window) {
    for (auto& [x, y] : previewPiece.shape) {
        previewGrid[x][y].setFillColor(previewPiece.color);
    }
    for (size_t i = 0; i < previewGrid.size(); i++) {
        for (const auto& square : previewGrid[i]) {
            window.draw(square);
        }
    }
    for (auto& [x, y] : previewPiece.shape) {
        previewGrid[x][y].setFillColor(sf::Color::Black);
    }
}

void Display::draw_text(sf::RenderWindow& window) {
    scoreText.setString("LINES: " + std::to_string(linesCleared));
    auto bounds = scoreText.getLocalBounds();
    scoreText.setOrigin({bounds.width/2 + bounds.left, 0});
    scoreText.setPosition({ width/2, height - 3*bounds.height });
    
    window.draw(scoreText);

    scoreText.setString("SCORE: " + std::to_string(score));
    bounds = scoreText.getLocalBounds();
    scoreText.setOrigin({bounds.width/2 + bounds.left, 0});
    scoreText.setPosition({ width/2, height - 5*bounds.height });
    
    window.draw(scoreText);

    if (game_over()) {
        scoreText.setString("GAME OVER");
        bounds = scoreText.getLocalBounds();
        scoreText.setOrigin({bounds.width/2 + bounds.left, 0});
        scoreText.setPosition({ width/2, 0 });
        
        window.draw(scoreText);


        scoreText.setCharacterSize(width/16);
        scoreText.setString("press R to restart");
        bounds = scoreText.getLocalBounds();
        scoreText.setOrigin({bounds.width/2 + bounds.left, 0});
        scoreText.setPosition({ width/2, width/12 });
        
        window.draw(scoreText);
        
        scoreText.setCharacterSize(width/12);
    }
}

void Display::draw_effects(sf::RenderWindow& window) {
    for (auto& effect : effects) 
        window.draw(effect);
}

bool Display::is_closing(const sf::Event& event) {
    return (event.type == sf::Event::Closed) 
        || (event.type == sf::Event::KeyPressed 
            && event.key.code == sf::Keyboard::Escape);
}

void Display::handle_event(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) { 
        switch(event.key.code) {
            case sf::Keyboard::Left: {
                if (piece_can_move_left() && !game_over()) {
                    piece.move_left();
                }
                break;
            }
            case sf::Keyboard::Right: {
                if (piece_can_move_right() && !game_over()) {
                    piece.move_right();
                }
                break;
            }
            case sf::Keyboard::Down: {
                if (piece_can_move_down() && !game_over()) {
                    piece.move_down();
                    frame = 1;
                }
                break;
            }
            case sf::Keyboard::Up: {
                if (piece_can_rotate() && !game_over()) {
                    piece.rotate();
                }
                break;
            }
            case sf::Keyboard::Space: {
                if (!game_over()) {
                    while (piece_can_move_down()) {
                        piece.move_down();
                    } 
                    frame = 0;

                    if (linesCleared > 50) {
                        score += 5;
                    }
                    else if (linesCleared > 10) {
                        score += 3;
                    }
                    else {
                        score += 1;
                    }
                }
                break;
            }
            case sf::Keyboard::D: {
                auto oldColor = blankColor;
                blankColor = (blankColor == sf::Color::White) ? 
                    sf::Color::Black : sf::Color::White;

                for (auto& line : grid) {
                    for (auto& square : line) {
                        if (square.getFillColor() == oldColor) {
                            square.setFillColor(blankColor);
                        }
                    }
                }

                break;
            }
            case sf::Keyboard::R: {
                for (auto& line : grid) {
                    for (auto& square : line) {
                        square.setFillColor(blankColor);
                    }
                }

                score = 0;
                linesCleared = 0;
                pieces = {};
                piece = TetrisPiece(generate_random_tetris_piece());
                previewPiece = TetrisPiece(generate_random_tetris_piece());
                effects = {};
                frame = 1;

                break;
            }
        
            default: break; 
        }
    }
}

void Display::update_frame() {
    auto stepTime = (linesCleared > 100) ? 6 
        : (linesCleared > 53) ? 7 
        : (60 - linesCleared);

    if (frame % stepTime == 0) {
        if (!piece_can_move_down()) {
            for (auto& [x, y] : piece.coords) {
                grid[x][y].setFillColor(piece.color);
            }

            piece = previewPiece;
            previewPiece = TetrisPiece(generate_random_tetris_piece());
        }
        else {
            piece.move_down();
        }
    }

    frame = (frame + 1) % stepTime;
}
void Display::update_score() {
    std::vector<uint32_t> scoreLines;

    auto filled = [this](const auto& square) {
        return square.getFillColor() != blankColor;
    };

    for (size_t i = 0; i < grid.size(); i++) {
        auto& line = grid[i];
        if (std::ranges::all_of(line, filled)) {
            scoreLines.insert(scoreLines.begin(), i);
            
            for (auto square : grid[i]) {
                square.setOutlineThickness(5);
                square.setOutlineColor(square.getFillColor());
                square.setFillColor(sf::Color::Transparent);
                
                auto bounds = square.getLocalBounds();
                square.setOrigin({bounds.width/2, bounds.width/2});
                square.move({bounds.width/2, bounds.height/2});
                effects.push_back(square);
            }
        }
    }

    linesCleared += scoreLines.size();
    score += scoreLines.size()*scoreLines.size();

    for (size_t i = 0; i < scoreLines.size(); i++) {
        shift_grid_down_to(scoreLines[i] + i);
    }
}


void Display::clean_up_effects() {
	erase_if(effects, [this](sf::RectangleShape effect) {
        return effect.getGlobalBounds().width > width*2 
            && effect.getGlobalBounds().height > height*2;
    });
}

void Display::update_effects() {
    float scaleSpeed = 1.05 + 0.005f*linesCleared;
    for (auto& effect : effects) {
        auto bounds = effect.getLocalBounds();
        effect.setOrigin({bounds.width/2, bounds.height/2});
        effect.scale({scaleSpeed, scaleSpeed});
        auto adjustedOutline = effect.getOutlineThickness()/scaleSpeed;
        effect.setOutlineThickness(adjustedOutline);
    }
}

bool Display::game_over() const {
    auto filled = [this](const auto& square) {
        return square.getFillColor() != blankColor;
    };
    auto& topLine = grid[4];
    bool topHasBlocks = std::ranges::any_of(topLine, filled);
    
    auto filledSquare = [this](const auto& coord) {
        return grid[coord.x][coord.y].getFillColor() != blankColor;
    };
    auto& coords = piece.coords;
    bool cannotSpawnSafely = std::ranges::any_of(coords, filledSquare);

    return topHasBlocks || cannotSpawnSafely;
}

void Display::shift_grid_down_to(uint32_t lineNumber) {
    for (int i = lineNumber; i > 4; i--) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            grid[i][j].setFillColor(grid[i-1][j].getFillColor());
        } 
        //grid[i].swap(grid[i-1]);
    }
}

Piece Display::generate_random_tetris_piece() {
    if (pieces.size() == 0) {
        pieces = { Piece::T, Piece::O, Piece::L, Piece::FlippedL, Piece::S, 
            Piece::FlippedS, Piece::Line };
            
        std::ranges::shuffle(pieces, generator);
    }

    auto back_piece = pieces.back();
    pieces.pop_back();
    return back_piece;
}

bool Display::piece_can_move_down() {
    for (auto [x, y] : piece.coords) {
        if (x >= 23 || grid[x+1][y].getFillColor() != blankColor) {
            return false;
        }
    }

    return true;
}

bool Display::piece_can_move_left() {
    for (auto [x, y] : piece.coords) {
        if (y <= 0 || grid[x][y-1].getFillColor() != blankColor) {
            return false;
        }
    }

    return true;
}

bool Display::piece_can_move_right() {
    auto emptyRightwardSpot = [this](auto coord) {
        auto [x, y] = coord;
        return (y < 9 && grid[x][y+1].getFillColor() == blankColor);
    };

    return std::ranges::all_of(piece.coords, emptyRightwardSpot);
}

bool Display::piece_can_rotate() {
    for (auto [x, y] : piece.rotated_shape()) {
        x += piece.origin.x;
        y += piece.origin.y;

        if (y > 9 || y < 0 || x > 23 || x < 0 
            || grid[x][y].getFillColor() != blankColor) {
            return false;
        }
    }

    return true;
}
