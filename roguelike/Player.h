#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Labyrinth.h"

struct Point {
    int x;
    int y;
};

enum class MovementDir {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Player {
    explicit Player(Point room_position = {.x = 0, .y = 0}, \
                           Labyrinth& game_labyrinth);

    bool Moved() const;
    void ProcessInput(MovementDir dir);
    void Draw(Image &screen);

private:
    Labyrinth labirinth;
    Point room_pos {.x = 0, .y = 0};
    Point coords {.x = 10, .y = 10};
    Point old_coords {.x = 10, .y = 10};
    int move_speed = 4;

};

#endif //MAIN_PLAYER_H
