#include "board.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

void require(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        std::exit(1);
    }
}

int main() {
    Board board;
    for (std::size_t type = 0; type < blocks.size(); ++type) {
        int occupied = 0;
        for (const auto& row : blocks[type]) {
            for (char cell : row) occupied += cell != ' ';
        }
        require(occupied == 4, "every shape has four cells");
        require(board.canPlace({type, 5, 1}), "every shape can spawn");
    }
    require(!board.canPlace({blocks.size(), 5, 1}), "invalid block type rejected");
    require(!board.canPlace({0, -1, 1}), "left wall blocks movement");
    require(!board.canPlace({0, W - 2, 1}), "right wall blocks movement");
    require(!board.canPlace({0, 5, H - 4}), "floor blocks movement");
    require(!board.canPlace({0, 5, 0}), "top wall blocks movement");

    Board spawnBlocked;
    spawnBlocked.place({0, 5, 1});
    require(!spawnBlocked.canPlace({0, 5, 1}), "blocked spawn detected");

    const Piece active{0, 5, 1};
    std::ostringstream activeOutput;
    board.draw(activeOutput, &active);
    require(activeOutput.str().find('I') != std::string::npos, "active piece is drawn");
    require(board.canPlace(active), "drawing does not lock the active piece");

    board.place({0, 0, H - 5});
    require(!board.canPlace({0, 0, H - 5}), "locked cells block movement");
    bool rejected = false;
    try {
        board.place({0, 0, H - 5});
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    require(rejected, "overlapping placement rejected");

    for (int col = 1; col <= 11; col += 2) {
        board.place({1, col, H - 4});
    }
    require(board.removeLines() == 2, "consecutive full lines removed");
    require(board.canPlace({1, 1, H - 4}), "cleared cells are empty");
    require(!board.canPlace({0, 0, H - 5}), "remaining cells move down");
    require(board.removeLines() == 0, "partial lines are preserved");

    std::ostringstream output;
    board.draw(output);
    const std::string rendered = output.str();
    require(rendered.substr(0, W) == std::string(W, '#'), "top wall remains intact");
    require(rendered.substr((H - 1) * (W + 1), W) == std::string(W, '#'),
            "bottom wall remains intact");
    require(board.canPlace({0, 5, 1}), "top row remains empty after clearing");

    std::cout << "Board tests passed\n";
}
