#include "Player.h"


Player::Player(Point room_position, \
               Labyrinth& game_labyrinth): room_pos(room_position), labyrinth(game_labyrinth)
{
    coords = labyrinth.GetPlayerPosByRoomPos(room_pos);
    old_coords = coords;
}


bool Player::Moved() const
{
    if (coords.x == old_coords.x && coords.y == old_coords.y) {
        return false;
    } else {
        return true;
    }
}


void Player::ProcessInput(MovementDir dir)
{    
    int move_dist = move_speed * 1;
    Point new_coords, check_coords, check_coords_2;
    
    switch(dir) {
        case MovementDir::UP:
            new_coords.x = coords.x;
            new_coords.y = coords.y + move_dist;
            check_coords.x = new_coords.x; 
            check_coords.y = new_coords.y + tile_size - 1;
            check_coords_2 = check_coords;
            check_coords_2.x += tile_size - 1;
            break;

        case MovementDir::DOWN:
            new_coords.x = coords.x;
            new_coords.y = coords.y - move_dist;
            check_coords = new_coords;
            check_coords_2 = check_coords;
            check_coords_2.x += tile_size - 1;
            break;
    
        case MovementDir::LEFT:
            new_coords.x = coords.x - move_dist;
            new_coords.y = coords.y;
            check_coords = new_coords;
            check_coords_2 = check_coords;
            check_coords_2.y += tile_size - 1;
            break;

        case MovementDir::RIGHT:
            new_coords.x = coords.x + move_dist;
            new_coords.y = coords.y;
            check_coords.x = new_coords.x + tile_size - 1;
            check_coords.y = new_coords.y;
            check_coords_2 = check_coords;
            check_coords_2.y += tile_size - 1;
            break;

        default:
            break;
    }

    if (check_coords.x >= 0 &&\
        check_coords.x <= tile_size * labyrinth.GetRoomWidth() - 1 &&\
        check_coords.y >= 0 &&\
        check_coords.y <= tile_size * labyrinth.GetRoomHeight() - 1 &&\
        labyrinth.GetTileTypeByPos(room_pos, check_coords) != SpriteType::WALL &&\
        labyrinth.GetTileTypeByPos(room_pos, check_coords_2) != SpriteType::WALL) {
        
        if (labyrinth.GetTileTypeByPos(room_pos, new_coords) == SpriteType::TOP_EXIT) {
            --room_pos.y;
            new_coords = labyrinth.GetPlayerPosByRoomPos(room_pos);
            coords = new_coords;
            old_coords = coords;
        } else if (labyrinth.GetTileTypeByPos(room_pos, new_coords) == SpriteType::RIGHT_EXIT) {
            ++room_pos.x;
            new_coords = labyrinth.GetPlayerPosByRoomPos(room_pos);
            coords = new_coords;
            old_coords = coords;
        } else if (labyrinth.GetTileTypeByPos(room_pos, new_coords) == SpriteType::BOTTOM_EXIT) {
            ++room_pos.y;
            new_coords = labyrinth.GetPlayerPosByRoomPos(room_pos);
            coords = new_coords;
            old_coords = coords;
        } else if (labyrinth.GetTileTypeByPos(room_pos, new_coords) == SpriteType::LEFT_EXIT) {
            --room_pos.x;
            new_coords = labyrinth.GetPlayerPosByRoomPos(room_pos);
            coords = new_coords;
            old_coords = coords;
        } else {
            old_coords = coords;
            coords = new_coords;
        }
    }
}


void Player::Draw(Image &screen)
{
    if (!finished_game && Moved()) {
        Image cur_room_img = labyrinth.GetRoomImgByPos(room_pos);
        for(int y = 0; y < screen.Height(); ++y) {
            for(int x = 0; x < screen.Width(); ++x) {
                screen.PutPixel(x, y, cur_room_img.GetPixel(x, y));
            }
        } 

        old_coords = coords;
       
        Point edge_coords_1 = coords;
        Point edge_coords_2 {.x = coords.x + tile_size - 1, .y = coords.y};
        Point edge_coords_3 {.x = coords.x, .y = coords.y + tile_size - 1};
        Point edge_coords_4 {.x = coords.x + tile_size - 1, .y = coords.y + tile_size - 1};
        bool into_blank_space = false;
        bool into_spikes_trap = false;
        Point into_spikes_trap_coords;

        if (labyrinth.GetTileTypeByPos(room_pos, edge_coords_1) == SpriteType::BLANK_SPACE ||\
            labyrinth.GetTileTypeByPos(room_pos, edge_coords_1) == SpriteType::BLANK_SPACE ||\
            labyrinth.GetTileTypeByPos(room_pos, edge_coords_1) == SpriteType::BLANK_SPACE ||\
            labyrinth.GetTileTypeByPos(room_pos, edge_coords_1) == SpriteType::BLANK_SPACE) {
            
            into_blank_space = true;
        }
        
        
        if (!into_blank_space) {
            for(int y = coords.y; y < coords.y + tile_size; y++) {
                for(int x = coords.x; x < coords.x + tile_size; x++) {
                    screen.PutPixel(x, y, labyrinth.GetSpriteImgByType(SpriteType::PLAYER).GetPixel(x - coords.x, y - coords.y));
                }
            }
        }
        
        if (labyrinth.GetTileTypeByPos(room_pos, coords) == SpriteType::SPIKES_TRAP) {
            Point start_pos {.x = (coords.x / tile_size) * tile_size, .y = (coords.y / tile_size) * tile_size};
            Image spikes_img = labyrinth.GetSpriteImgByType(SpriteType::SPIKES_TRAP);
            for (int y = start_pos.y; y < start_pos.y + tile_size; y++) {
                for (int x = start_pos.x; x < start_pos.x + tile_size; x++) {
                    screen.PutPixel(x, y, spikes_img.GetPixel(x % tile_size, y % tile_size));
                }
            }
        }
      
        if (labyrinth.GetTileTypeByPos(room_pos, coords) == SpriteType::BLANK_SPACE ||\
            labyrinth.GetTileTypeByPos(room_pos, coords) == SpriteType::SPIKES_TRAP) {
            
            int window_width = tile_size * labyrinth.GetRoomWidth();
            int window_height = tile_size * labyrinth.GetRoomHeight();
            Image lost_text_img = labyrinth.GetSpriteImgByType(SpriteType::LOST_TEXT);
            int lost_text_width = lost_text_img.Width();
            int lost_text_height = lost_text_img.Height();

            for (int y = window_height / 2 - lost_text_height / 2;
                  y < window_height / 2 + lost_text_height / 2;
                  y++) {
                for (int x = window_width / 2 - lost_text_width / 2;
                       x < window_width / 2 + lost_text_width / 2;
                       x++) {
                    screen.PutPixel(x, y, lost_text_img.GetPixel(x - (window_width / 2 - lost_text_width / 2), \
                                                                                y - (window_height / 2 - lost_text_height / 2)));
                }
            }
          
            finished_game = true;
        }
      
        if (labyrinth.GetTileTypeByPos(room_pos, coords) == SpriteType::QUIT) {
            int window_width = tile_size * labyrinth.GetRoomWidth();
            int window_height = tile_size * labyrinth.GetRoomHeight();
            Image won_text_img = labyrinth.GetSpriteImgByType(SpriteType::WON_TEXT);
            int won_text_width = won_text_img.Width();
            int won_text_height = won_text_img.Height();

            for (int y = window_height / 2 - won_text_height / 2;
                  y < window_height / 2 + won_text_height / 2;
                  y++) {
                for (int x = window_width / 2 - won_text_width / 2;
                       x < window_width / 2 + won_text_width / 2;
                       x++) {
                    screen.PutPixel(x, y, won_text_img.GetPixel(x - (window_width / 2 - won_text_width / 2), \
                                                                                y - (window_height / 2 - won_text_height / 2)));
                }
            }
          
            finished_game = true;
        }
    }
}
