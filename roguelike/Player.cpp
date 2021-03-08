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
    Point new_coords, check_coords;
    switch(dir) {
        case MovementDir::UP:
            new_coords.x = coords.x;
            new_coords.y = coords.y + move_dist;
            check_coords.x = new_coords.x; 
            check_coords.y = new_coords.y + tile_size;
            //old_coords.y = coords.y;
            //coords.y += move_dist;
            break;

        case MovementDir::DOWN:
            new_coords.x = coords.x;
            new_coords.y = coords.y - move_dist;
            check_coords = new_coords;
            //old_coords.y = coords.y;
            //coords.y -= move_dist;
            break;
    
        case MovementDir::LEFT:
            new_coords.x = coords.x - move_dist;
            new_coords.y = coords.y;
            check_coords = new_coords;
            //old_coords.x = coords.x;
            //coords.x -= move_dist;
            break;

        case MovementDir::RIGHT:
            new_coords.x = coords.x + move_dist;
            new_coords.y = coords.y;
            check_coords.x = new_coords.x + tile_size;
            check_coords.y = new_coords.y;
            //old_coords.x = coords.x;
            //coords.x += move_dist;
            break;

        default:
            break;
    }

    if (new_coords.x >= 0 &&\
        new_coords.x < tile_size * (labyrinth.GetRoomWidth() - 1) &&\
        new_coords.y >= 0 &&\
        new_coords.y < tile_size * (labyrinth.GetRoomHeight() - 1) &&\
        labyrinth.GetTileTypeByPos(room_pos, check_coords) != SpriteType::WALL) {
        
        old_coords = coords;
        coords = new_coords;
    }
}


void Player::Draw(Image &screen)
{
    if (Moved()) {
        Image cur_room_img = labyrinth.GetRoomImgByPos(room_pos);
        for(int y = 0; y < screen.Height(); ++y) {
            for(int x = 0; x < screen.Width(); ++x) {
                screen.PutPixel(x, y, cur_room_img.GetPixel(x, y));
            }
        } 

        old_coords = coords;
    

        for(int y = coords.y; y < coords.y + tile_size; y++) {
            for(int x = coords.x; x < coords.x + tile_size; x++) {
                screen.PutPixel(x, y, labyrinth.GetSpriteImgByType(SpriteType::PLAYER).GetPixel(x - coords.x, y - coords.y));
            }
        } 
    }
}
