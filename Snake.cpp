//
// Created by Kristan Panton on 8/8/2020.
//

#include "Snake.h"

/*
 * TODO:
 * */

Snake::Snake(SDL_Renderer *ren, int snake_xpos, int snake_ypos, int size, int len, int window_xpos, int window_ypos,
             int window_width, int window_height) {
    renderer = ren;
    block_side_length = size;
    length = len;

    window_x = window_xpos;
    window_y = window_ypos;
    this->window_width = window_width;
    this->window_height = window_height;

    for (int i = len - 1; i >= 0; i--) {
        body.push_back({snake_xpos + (size * i), snake_ypos, size, size});
//        printf("x: %d, y: %d, w: %d, h: %d\n", snake_xpos + (size * i), snake_ypos, size, size);
    }
}

void Snake::render() {
    if (is_dead)
        SDL_SetRenderDrawColor(renderer, dead_color.r, dead_color.g, dead_color.b, dead_color.a);
//        setRenderColor(renderer, dead_color);
    else
        SDL_SetRenderDrawColor(renderer, alive_color.r, alive_color.g, alive_color.b, alive_color.a);
//        setRenderColor(renderer, alive_color);

    SDL_RenderFillRect(renderer, &body[0]);

    int len = body.size();

    for (int i = 1; i < len; i++)
        SDL_RenderDrawRect(renderer, &body[i]);
}

void Snake::move(const int direction) {
    if (!is_dead && can_move) {
        // Distance to offset each block
        current_direction = direction;
        int delta_x = 0;
        int delta_y = 0;

        switch (direction) {
            case SDLK_UP:
                delta_y = -block_side_length;
                break;

            case SDLK_DOWN:
                delta_y = block_side_length;
                break;

            case SDLK_LEFT:
                delta_x = -block_side_length;
                break;

            case SDLK_RIGHT:
                delta_x = block_side_length;
                break;

            default:
                return;
        }

        int new_head_x = getHead().x + delta_x;
        int new_head_y = getHead().y + delta_y;

        body.insert(body.begin(), {new_head_x, new_head_y, block_side_length, block_side_length});

        // ~~~~~~~~~~< Border Collision >~~~~~~~~~~
        for (int i = 0; i < length; i++) {
            if (body[0].x == body[i].x && body[0].y == body[i].y && i != 0)
                killSnake();
            else {
                // If snake passes game window border, move each segment to opposite side of game window
                if (body[i].x < window_x)
                    body[i].x += window_width;
                else if (body[i].x >= window_width)
                    body[i].x -= window_width;
                if (body[i].y < window_y)
                    body[i].y += window_height;
                else if (body[i].y >= window_height)
                    body[i].y -= window_height;
            }
        }

        // Kills snake if it moves past the game window borders
//        if (getHead().x < window_x || getHead().x >= window_width || getHead().y < window_y || getHead().y >= window_height) {
//            killSnake();
//        }
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // If the number of segments in snake is longer than the supposed length,
        // store a copy then delete tail
        if (body.size() > length) {
            tail = body.back();
            body.pop_back();
        }
    }
}

void Snake::killSnake() {
    is_dead = true;

    // Erases head so the head isn't inside the snakes body
    body.erase(body.begin());
    // Pushes back tail so the length of the snake remains the same
    body.push_back(tail);

    puts("Game Over.");
}
