#pragma one;

#include <deque>
#include <List>


enum class Direction {
    Up, Down, Left, Right
};

enum class GameState {
    Running, Win, GameOver
};

struct Coordinates
{
    int col;
    int row;
    bool operator== (const Coordinates &a) const;
};

class SnakeEngine
{
private:

    bool generateFood();
    bool hitBody(const Coordinates &pos) const;
    bool hitWall(const Coordinates &pos) const;
    void createSnake();

public:
    std::deque<Coordinates> snakeBody;
    Direction headDirection;
    Coordinates food;
    int score;
    int cols, rows;
    GameState state;

    SnakeEngine(int cols, int rows);

    void newGame();
    void update(const Direction &input);
};


