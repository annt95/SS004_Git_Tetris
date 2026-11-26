#include "board.h"

#include <ostream>
#include <stdexcept>

const std::array<Block, 7> blocks = {{
    {{{' ', 'I', ' ', ' '}, {' ', 'I', ' ', ' '},
      {' ', 'I', ' ', ' '}, {' ', 'I', ' ', ' '}}},
    {{{' ', ' ', ' ', ' '}, {' ', 'O', 'O', ' '},
      {' ', 'O', 'O', ' '}, {' ', ' ', ' ', ' '}}},
    {{{' ', ' ', ' ', ' '}, {' ', 'T', ' ', ' '},
      {'T', 'T', 'T', ' '}, {' ', ' ', ' ', ' '}}},
    {{{' ', ' ', ' ', ' '}, {' ', 'S', 'S', ' '},
      {'S', 'S', ' ', ' '}, {' ', ' ', ' ', ' '}}},
    {{{' ', ' ', ' ', ' '}, {'Z', 'Z', ' ', ' '},
      {' ', 'Z', 'Z', ' '}, {' ', ' ', ' ', ' '}}},
    {{{' ', ' ', ' ', ' '}, {'J', ' ', ' ', ' '},
      {'J', 'J', 'J', ' '}, {' ', ' ', ' ', ' '}}},
    {{{' ', ' ', ' ', ' '}, {' ', ' ', 'L', ' '},
      {'L', 'L', 'L', ' '}, {' ', ' ', ' ', ' '}}}
}};

Board::Board() {
    for (int row = 0; row < H; ++row) {
        for (int col = 0; col < W; ++col) {
            cells_[row][col] = (row == 0 || row == H - 1 || col == 0 || col == W - 1)
                                   ? '#' : ' ';
        }
    }
}

bool Board::canPlace(const Piece& piece) const {
    if (piece.type >= blocks.size()) {
        return false;
    }
    for (int row = 0; row < BLOCK_SIZE; ++row) {
        for (int col = 0; col < BLOCK_SIZE; ++col) {
            if (blocks[piece.type][row][col] == ' ') {
                continue;
            }
            const int x = piece.x + col;
            const int y = piece.y + row;
            if (x < 1 || x >= W - 1 || y < 1 || y >= H - 1 || cells_[y][x] != ' ') {
                return false;
            }
        }
    }
    return true;
}

void Board::place(const Piece& piece) {
    if (!canPlace(piece)) {
        throw std::invalid_argument("Cannot place piece on the board");
    }
    for (int row = 0; row < BLOCK_SIZE; ++row) {
        for (int col = 0; col < BLOCK_SIZE; ++col) {
            const char cell = blocks[piece.type][row][col];
            if (cell != ' ') {
                cells_[piece.y + row][piece.x + col] = cell;
            }
        }
    }
}

int Board::removeLines() {
    int removed = 0;
    for (int row = H - 2; row > 0; --row) {
        bool full = true;
        for (int col = 1; col < W - 1; ++col) {
            if (cells_[row][col] == ' ') {
                full = false;
                break;
            }
        }
        if (!full) {
            continue;
        }
        for (int dest = row; dest > 1; --dest) {
            for (int col = 1; col < W - 1; ++col) {
                cells_[dest][col] = cells_[dest - 1][col];
            }
        }
        for (int col = 1; col < W - 1; ++col) {
            cells_[1][col] = ' ';
        }
        ++removed;
        ++row;
    }
    return removed;
}

void Board::draw(std::ostream& out, const Piece* active) const {
    auto display = cells_;
    if (active != nullptr && canPlace(*active)) {
        for (int row = 0; row < BLOCK_SIZE; ++row) {
            for (int col = 0; col < BLOCK_SIZE; ++col) {
                const char cell = blocks[active->type][row][col];
                if (cell != ' ') {
                    display[active->y + row][active->x + col] = cell;
                }
            }
        }
    }
    for (const auto& row : display) {
        for (char cell : row) {
            out << cell;
        }
        out << '\n';
    }
}
