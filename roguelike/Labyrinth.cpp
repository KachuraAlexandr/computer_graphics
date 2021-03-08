#include "Labyrinth.h"


Labyrinth::Labyrinth(const std::string &labyrinth_path,\
                     const std::vector<std::string> &room_paths,\
                     const std::vector<std::string> &sprite_paths)
{
    std::ifstream labyrinth_f;
    try {
        labyrinth_f.open(labyrinth_path);
        if (!labyrinth_f) {
            throw std::runtime_error(std::strerror(errno));
        }
    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
    }

    std::ostringstream ss;
    ss << labyrinth_f.rdbuf();
    labyrinth_plan = ss.str();
    ss.str("");
    ss.clear();
    labyrinth_width = labyrinth_plan.find_first_of('\n');
    labyrinth_plan.erase(std::remove(labyrinth_plan.begin(),\
                                     labyrinth_plan.end(), '\n'),\
                         labyrinth_plan.end());
    labyrinth_height = labyrinth_plan.length() / labyrinth_width;
    
    for (auto& room_path: room_paths) {
        std::ifstream room_f;
        try {
            room_f.open(room_path);
            if (!room_f) {
                throw std::runtime_error(std::strerror(errno));
            }
        } catch (std::exception const& e) {
            std::cout << e.what() << std::endl;
        }
        
        ss << room_f.rdbuf();
        room_plans.push_back(ss.str());
        ss.str("");
        ss.clear();
        room_f.close();
    }
    
    room_width = room_plans[0].find_first_of('\n');
    for (auto& room_plan: room_plans) {
        room_plan.erase(std::remove(room_plan.begin(),\
                                    room_plan.end(), '\n'),\
                        room_plan.end());
    }
    room_height = room_plans[0].length() / room_width;
    
    for (auto& sprite_path: sprite_paths) {
        Image cur_sprite(sprite_path);
        int sprite_width = cur_sprite.Width();
        int sprite_height = cur_sprite.Height();
        for (int y = 0; y < sprite_height / 2; y++) {
            for (int x = 0; x < sprite_width; x++) {
                cur_sprite.PutPixel(x, sprite_height - y - 1, cur_sprite.GetPixel(x, y));
            }
        }
        sprite_imgs.push_back(cur_sprite);
    }

    for (auto& room_plan: room_plans) {
        Image room_img(room_height * tile_size,\
                       room_width * tile_size, 4);
        for (int h = 0; h < room_height; h++) {
            for (int w = 0; w < room_width; w++) {
                SpriteType cur_sprite_type;
                switch (room_plan[h * room_width + w]) {
                    case '.':

                    case 'T':

                    case '@':
                        cur_sprite_type = SpriteType::GROUND;
                        break;
                        
                    case '#':
                        cur_sprite_type = SpriteType::WALL;
                        break;
                        
                    case ' ':
                        cur_sprite_type = SpriteType::BLANK_SPACE;
                        break;

                    case 'x':
                        if (h == 0) {
                            cur_sprite_type = SpriteType::TOP_EXIT;
                        } else if (w == room_width - 1) {
                            cur_sprite_type = SpriteType::RIGHT_EXIT;
                        } else if (h == room_height - 1) {
                            cur_sprite_type = SpriteType::BOTTOM_EXIT;
                        } else if (w == 0) {
                            cur_sprite_type = SpriteType::LEFT_EXIT;
                        }
                        break;
                    
                    case 'Q':
                        cur_sprite_type = SpriteType::QUIT;
                        break;

                    default:
                        break;
                }

                for (int y = 0; y < tile_size; y++) {
                    for (int x = 0; x < tile_size; x++) {
                        room_img.PutPixel(w * tile_size + x,\
                                          h * tile_size + y,\
                                          sprite_imgs[static_cast<int>\
                                            (cur_sprite_type)].GetPixel(x, y));
                    }
                }
            }
        }
        room_imgs.push_back(room_img);
    }
}
 

Image&
Labyrinth::GetRoomImgByPos(Point room_pos)
{
    return room_imgs[int(labyrinth_plan[room_pos.y *\
                                 labyrinth_width + room_pos.x]) -\
                                 int('A')];
}


Image&
Labyrinth::GetSpriteImgByType(SpriteType sprite_type)
{
    return sprite_imgs[static_cast<int>(sprite_type)];
}


Point
Labyrinth::GetPlayerPosByRoomPos(Point room_pos)
{
    int player_tile_pos = room_plans[int(labyrinth_plan[room_pos.y *\
                               labyrinth_width + room_pos.x]) -\
                               int('A')].find_first_of('@');
    int x_pos = (player_tile_pos % room_width) * tile_size;
    int y_pos = (room_height - 1 - player_tile_pos / room_width) * tile_size;
    Point player_pos{.x = x_pos, .y = y_pos};
    return player_pos;
}


SpriteType
Labyrinth::GetTileTypeByPos(Point room_pos, Point coords)
{
    char room_type = labyrinth_plan[room_pos.y * \
                                 labyrinth_width + room_pos.x];
    std::string room_plan = room_plans[int(room_type) - int('A')];
    int room_plan_pos = coords.y / tile_size * room_width + \
                                     coords.x / tile_size;
    if (room_plan[room_plan_pos] == '.') {
        return SpriteType::GROUND;
    } else if (room_plan[room_plan_pos] == '#') {
        return SpriteType::WALL;
    } else if (room_plan[room_plan_pos] == ' ') {
        return SpriteType::BLANK_SPACE;
    } else if (room_plan[room_plan_pos] == 'T') {
        return SpriteType::SPIKES_TRAP;
    } else if (room_plan[room_plan_pos] == 'x') {
        if (coords.y / tile_size == room_height - 1) {
            return SpriteType::TOP_EXIT;
        } else if (coords.x / tile_size == room_width - 1) {
            return SpriteType::RIGHT_EXIT;
        } else if (coords.y / tile_size == 0) {
            return SpriteType::BOTTOM_EXIT;
        } else if (coords.x / tile_size == 0) {
            return SpriteType::LEFT_EXIT;
        }
    } else if (room_plan[room_plan_pos] == '@') {
        return SpriteType::PLAYER;
    }
}


int
Labyrinth::GetRoomWidth()
{
    return room_width;
}


int
Labyrinth::GetRoomHeight()
{
    return room_height;
}
