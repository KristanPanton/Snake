//
// Created by Kristan Panton on 8/8/2020.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <SDL.h>
#include <vector>


class Snake {
public:
    Snake(SDL_Renderer *ren, int snake_xpos, int snake_ypos, int size, int len, int window_xpos, int window_ypos,
          int window_width, int window_height);

    ~Snake() = default;

    // Appends a block to the snake with the appropriate coordinates
    void grow() { length++; }

    // Changes the coordinates of each snake segment
    void move(int direction); // Moves snake

    // Renders the snake
    void render();

    // Returns the number of blocks in the snakes body
    size_t getLength() const { return length; }

    // Returns the head of the snake body
    SDL_Rect getHead() const { return body.front(); }

    int getCurrentDirection() const { return current_direction; }

    std::vector<SDL_Rect> &getBody() { return body; }

    // Returns the state of snake
    bool isDead() const { return is_dead; }

    void killSnake();

    void reviveSnake() { is_dead = false; };

    bool canMove() const { return can_move; };

    void enableMovement() { can_move = true; }

    void disableMovement() { can_move = false; }

private:
    SDL_Renderer *renderer;

    int window_x;
    int window_y;
    int window_width;
    int window_height;

    std::vector<SDL_Rect> body; // The body of the snake
    SDL_Rect tail;
    SDL_Color alive_color = {0xff, 0xff, 0xff, 0xff};
    SDL_Color dead_color = {0xff, 0x00, 0x00, 0xff};
    int block_side_length = 0; // The length of the side of snake block
    int length = 0; // The length of the snake

    int current_direction = 0;

    bool is_dead = false;
    bool can_move = true;

    void setRenderColor(SDL_Renderer *ren, SDL_Color col) { SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, col.a); }
};


#endif //SNAKE_SNAKE_H
