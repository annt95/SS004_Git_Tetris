#include "board.h"

#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdexcept>
#include <thread>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#else
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace {
volatile std::sig_atomic_t interrupted = 0;

void onSignal(int) {
    interrupted = 1;
}

class Terminal {
public:
    Terminal() {
#ifdef _WIN32
        if (!_isatty(_fileno(stdin)) || !_isatty(_fileno(stdout))) {
            throw std::runtime_error("Run the game in an interactive terminal");
        }
#else
        if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
            throw std::runtime_error("Run the game in an interactive terminal");
        }
        if (tcgetattr(STDIN_FILENO, &original_) != 0) {
            throw std::runtime_error("Cannot read terminal settings");
        }
        termios raw = original_;
        raw.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
        if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0) {
            throw std::runtime_error("Cannot configure terminal");
        }
#endif
    }

    ~Terminal() {
#ifndef _WIN32
        tcsetattr(STDIN_FILENO, TCSANOW, &original_);
#endif
    }

    int readKey() const {
#ifdef _WIN32
        return _kbhit() ? _getch() : -1;
#else
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(STDIN_FILENO, &readSet);
        timeval timeout = {0, 0};
        const int ready = select(STDIN_FILENO + 1, &readSet, nullptr, nullptr, &timeout);
        if (ready < 0 && !interrupted) {
            throw std::runtime_error("Cannot read terminal input");
        }
        if (ready <= 0) {
            return -1;
        }
        unsigned char key;
        const ssize_t count = read(STDIN_FILENO, &key, 1);
        if (count != 1) {
            throw std::runtime_error("Terminal input closed");
        }
        return key;
#endif
    }

private:
#ifndef _WIN32
    termios original_{};
#endif
};

void draw(const Board& board, const Piece* active, int lines) {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
    board.draw(std::cout, active);
    std::cout << "a: left  d: right  x: down  q: quit\n"
              << "Lines: " << lines << '\n' << std::flush;
}
}

int main() {
    try {
        Terminal terminal;
        std::signal(SIGINT, onSignal);
        std::signal(SIGTERM, onSignal);

        Board board;
        std::mt19937 random(std::random_device{}());
        std::uniform_int_distribution<std::size_t> choose(0, blocks.size() - 1);
        Piece piece{choose(random), 5, 1};
        int lines = 0;
        auto nextDrop = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        draw(board, &piece, lines);

        while (!interrupted) {
            const int key = terminal.readKey();
            if (key == 'q') {
                break;
            }
            Piece candidate = piece;
            if (key == 'a') --candidate.x;
            if (key == 'd') ++candidate.x;
            if (key == 'x') ++candidate.y;
            if ((key == 'a' || key == 'd' || key == 'x') && board.canPlace(candidate)) {
                piece = candidate;
                draw(board, &piece, lines);
            }

            const auto now = std::chrono::steady_clock::now();
            if (now >= nextDrop) {
                candidate = piece;
                ++candidate.y;
                if (board.canPlace(candidate)) {
                    piece = candidate;
                } else {
                    board.place(piece);
                    lines += board.removeLines();
                    piece = {choose(random), 5, 1};
                    if (!board.canPlace(piece)) {
                        draw(board, nullptr, lines);
                        std::cout << "Game over!\n";
                        break;
                    }
                }
                draw(board, &piece, lines);
                nextDrop = now + std::chrono::milliseconds(500);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
