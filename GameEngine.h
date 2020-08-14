//
// Created by Kristan Panton on 8/7/2020.
//

#ifndef SNAKE_GAMEENGINE_H
#define SNAKE_GAMEENGINE_H
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>


class GameEngine {
public:
    GameEngine() = default;

    ~GameEngine() = default;

    void init(const char *title, int xpos, int ypos, int w_width, int w_height, int gw_xpos, int gw_ypos, int gw_width,
              int gw_height, bool fullscreen);

    void handleEvents();

    void update();

    void render();

    void clean();

    bool running() const { return isRunning; }


private:
    // Helper function to generate random coordinates for the prey
    void generateRandomCoordinate(SDL_Rect &rect) const;

    void reinit();

    // Ensures that prey coordinates are not within snake
    void validatePreyCoordinates(SDL_Rect &rect);

    bool isRunning;
    bool gameOver;

    SDL_Window *window;
    int window_width = 0;
    int window_height = 0;
    int game_window_x = 0;
    int game_window_y = 0;
    int game_window_width;
    int game_window_height;

    SDL_Renderer *renderer;
};


#endif //SNAKE_GAMEENGINE_H
