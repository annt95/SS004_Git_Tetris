#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#include <array>
#include <cstddef>
#include <iosfwd>

constexpr int H = 20;
constexpr int W = 15;
constexpr int BLOCK_SIZE = 4;

using Block = std::array<std::array<char, BLOCK_SIZE>, BLOCK_SIZE>;

extern const std::array<Block, 7> blocks;

struct Piece {
    std::size_t type;
    int x;
    int y;
};

class Board {
public:
    Board();

    bool canPlace(const Piece& piece) const;
    void place(const Piece& piece);
    int removeLines();
    void draw(std::ostream& out, const Piece* active = nullptr) const;

private:
    std::array<std::array<char, W>, H> cells_;
};

#endif
