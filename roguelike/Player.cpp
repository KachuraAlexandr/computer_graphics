#include "Player.h"


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
    switch(dir) {
        case MovementDir::UP:
            old_coords.y = coords.y;
            coords.y += move_dist;
            break;

        case MovementDir::DOWN:
            old_coords.y = coords.y;
            coords.y -= move_dist;
            break;
    
        case MovementDir::LEFT:
            old_coords.x = coords.x;
            coords.x -= move_dist;
            break;

        case MovementDir::RIGHT:
            old_coords.x = coords.x;
            coords.x += move_dist;
            break;

        default:
            break;
    }
}


void Player::Draw(Image &screen, Image &tile)
{
    if (Moved()) {
        for(int y = old_coords.y; y <= old_coords.y + tile_size; ++y) {
            for(int x = old_coords.x; x <= old_coords.x + tile_size; ++x) {
                screen.PutPixel(x, y, tile.GetPixel(x, y));
            }
        }

        old_coords = coords;
    }

    for(int y = coords.y; y <= coords.y + tile_size; ++y) {
        for(int x = coords.x; x <= coords.x + tile_size; ++x) {
            screen.PutPixel(x, y, player_img.GetPixel(x % tile_size, y % tile_size));
        }
    }
}
