#include "Labyrinth.h"


Labyrinth::Labyrinth(std::string &labyrinth_path, \ 
                     std::vector<std::string> &room_paths, \ 
                     std::vector<std::string> &sprite_paths)
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
                                     labyrinth.end(), '\n'),\
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
    }
    
    room_width = rooms_plan[0].find_first_of('\n');
    for (auto& room_plan: room_plans) {
        room_plan.erase(std::remove(room_plan.begin(),\
                                    room_plan.end(), '\n'), \
                        room_plan.end());
    }
    room_height = room_plans[0].length() / room_width;

    for (auto& sprite_path: sprite_paths) {
        Image cur_sprite(sprite_path);
        sprite_imgs.push_back(cur_sprite);
    }

    for (auto& room_plan: room_plans) {
        Image room_img(room_height * tile_size, \
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
                        
                    default:
                        break;
                }

                for (int y = 0; y < tile_size; y++) {
                    for (int x = 0; x < tile_size; x++) {
                        room_img.PutPixel(w * tile_size + x, \
                                          h * tile_size + y, \    
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
    return room_imgs[ord(labyrinth_plan[room_pos.y * \
                                 labyrinth_width + room_pos.x]) - \
                                 ord('A')];
}


Image&
Labyrinth::GetSpriteImgByType(SpriteType sprite_type)
{
    return sprite_imgs[static_cast<int>(sprite_type)];
}


Point
Labyrinth::GetPlayerPosByRoomPos(Point room_pos)
{
    int player_tile_pos = room_plans[ord(labyrinth_plan[room_pos.y * \
                               labyrinth_width + room_pos.x]) - \
                               ord('A')].find_first_of('@');
    return Point{.x = (player_tile_pos % room_width) * tile_size, \
                        .y = (room_height - player_tile_pos // room_width) * tile_size};
}
