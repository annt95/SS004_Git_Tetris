# SS004 Git Tetris

Trò chơi xếp hình chạy trong terminal, phát triển từ mã C++ ban đầu.

## Yêu cầu

- Trình biên dịch hỗ trợ C++17.
- CMake 3.16 trở lên (tùy chọn nếu biên dịch trực tiếp).
- Terminal tương tác trên macOS, Linux hoặc Windows.

## Biên dịch và chạy

Với CMake:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Chạy `./build/tetris` trên macOS/Linux hoặc `build\Debug\tetris.exe` trên Windows
(đường dẫn có thể khác tùy cấu hình CMake).

Trên macOS/Linux cũng có thể biên dịch trực tiếp:

```sh
c++ -std=c++17 -Isrc src/main.cpp src/board.cpp -o tetris
./tetris
```

## Điều khiển

- `a`: sang trái; `d`: sang phải.
- `x`: di chuyển xuống một ô.
- `q`: thoát.

Khối tự rơi mỗi 0,5 giây. Hàng đầy được xóa và trò chơi kết thúc khi
không còn chỗ xuất hiện khối mới. Phiên bản này chưa hỗ trợ xoay khối.
