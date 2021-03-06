#ifndef MAIN_LABYRINTH_H
#define MAIN_LABYRINTH_H

#include <iostream>
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
    PLAYER,
    WIN_TEXT,
    LOSE_TEXT
}


struct Labyrinth {
    Labyrinth(std::string &labyrinth_path, std::vector<std::string> &room_paths,    
              std::vector<std::string> &sprite_paths);
    Image& GetRoomImgByPos(Point room_pos);
    Image& GetSpriteImgByType(SpriteType sprite_type);
    Point GetPlayerPosByRoomPos(Point room_pos);

private:
    std::string labyrinth_plan;
    std::vector<std::string> room_plans;
    std::vector<Image> room_imgs;
    std::vector<Image> sprites;
    int labyrinth_height;
    int labyrinth_width;
    int room_height;
    int room_width;
};

#endif //MAIN_LABYRINTH_H
