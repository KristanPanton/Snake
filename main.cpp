#include "GameEngine.h"

GameEngine *game_engine = nullptr;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 500;

const int GAME_WINDOW_WIDTH = 600;
const int GAME_WINDOW_HEIGHT = 400;
const int GAME_WINDOW_X = 0;
const int GAME_WINDOW_Y = 0;
bool fullscreen = false;

int main(int argc, const char *argv[]) {
    const int FPS = 60;
    const int frame_delay = 1000 / FPS;

    Uint32 frame_start;
    unsigned int frame_time;

    game_engine = new GameEngine();

    game_engine->init("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
                      GAME_WINDOW_X, GAME_WINDOW_Y, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, fullscreen);

    while (game_engine->running()) {
        frame_start = SDL_GetTicks();

        game_engine->handleEvents();
        game_engine->update();
        game_engine->render();

        frame_time = SDL_GetTicks() - frame_start;

        if (frame_delay > frame_time)
            SDL_Delay(frame_delay - frame_time);
    }

    game_engine->clean();

    return 0;
}