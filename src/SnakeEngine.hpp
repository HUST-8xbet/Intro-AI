#pragma once

#include <deque>
#include <List>

// NOTE class SnakeEngine bao gồm đầy đủ các thông tin về trạng thái game con răns
// snakeBody là một hàng đợi, mỗi phần tử tương ứng với tọa độ một điểm trong thân rắn
// tọa độ đầu rắn là phần tử đầu hàng đợi, lấy bằng hàm snakeBody.front()
// int cols, rows là số hàng số cột trong game

enum class Direction {
    Up, Down, Left, Right
};

enum class GameState {
    Running, Win, GameOver
};

// NOTE Chú ý quan trọng về hệ trục tọa độ:
// col (cột thứ) tương ứng với trục Ox hướng từ TRÁI sang PHẢI
// row (hàng thứ) tương ứng với truck Oy hướng từ TRÊN xuống DƯỚI (để giống chuẩn của SFML)
// VD tọa độ các ô trong một bảng 4x4:
//  {0, 0}  {1, 0}  {2, 0}  {3, 0}
//  {0, 1}  {1, 1}  {2, 1}  {3, 1}
//  {0, 2}  {1, 2}  {2, 2}  {3, 2}
//  {0, 3}  {1, 3}  {2, 3}  {3, 3}
struct Coordinates
{
    int col;
    int row;
    bool operator== (const Coordinates &a) const{
        return (col == a.col) && (row == a.row);
    };
};


class SnakeEngine
{
private:

    bool generateFood();
    void createSnake();

public:

    std::deque<Coordinates> snakeBody;
    Direction headDirection;
    Coordinates food;
    int step;
    int step_since_last_food;
    int score;
    int cols, rows;
    GameState state;

    SnakeEngine(int cols, int rows);

    void newGame();
    void update(const Direction &input);
    bool hitBody(const Coordinates &pos) const;
    bool hitWall(const Coordinates &pos) const;
};


