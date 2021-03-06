#ifndef MAIN_LABYRINTH_H
#define MAIN_LABYRINTH_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Image.h"


struct Point {
    int x;
    int y;
};

enum class SpriteType {
    GROUND,
    WALL,
    BLANK_SPACE,
    SPIKES_TRAP,
    TOP_EXIT,
    RIGHT_EXIT,
    BOTTOM_EXIT,
    LEFT_EXIT,
    QUIT,
    PLAYER,
    WON_TEXT,
    LOST_TEXT
};


struct Labyrinth {
    Labyrinth(const std::string &labyrinth_path,\
              const std::vector<std::string> &room_paths,\
              const std::vector<std::string> &sprite_paths);
    Image& GetRoomImgByPos(Point room_pos);
    Image& GetSpriteImgByType(SpriteType sprite_type);
    Point GetPlayerPosByRoomPos(Point room_pos);
    SpriteType GetTileTypeByPos(Point room_pos, Point coords);
    int GetRoomWidth();
    int GetRoomHeight();

private:
    std::string labyrinth_plan;
    std::vector<std::string> room_plans;
    std::vector<Image> room_imgs;
    std::vector<Image> sprite_imgs;
    int labyrinth_height;
    int labyrinth_width;
    int room_height;
    int room_width;
};

#endif //MAIN_LABYRINTH_H
