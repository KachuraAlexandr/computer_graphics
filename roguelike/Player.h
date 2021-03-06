#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Labyrinth.h"


enum class MovementDir {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Player {
    explicit Player(Point room_position, \
                           Labyrinth& game_labyrinth);

    bool Moved() const;
    void ProcessInput(MovementDir dir);
    void Draw(Image &screen);

private:
    Labyrinth labirinth;
    Point room_pos;
    Point coords;
    Point old_coords;
    int move_speed = 4;

};

#endif //MAIN_PLAYER_H
