//
// Created by Kristan Panton on 8/7/2020.
//

#include "GameEngine.h"
#include "Snake.h"

/*
 * TODO:
 *  - Tidy up and refactor code
 * */

Snake *snake;
SDL_Rect prey;
SDL_Rect game_window_rect;
SDL_Event event;
int snake_direction = 0;
int snake_segment_size = 20;
int snake_len = 4;
//int snake_ypos = ;
//int snake_xpos = ;

SDL_Color white = {0xff, 0xff, 0xff, 0xff};
SDL_Rect score_rect;

TTF_Font *blockway_font = nullptr;
SDL_Surface *score_surface = nullptr;
SDL_Texture *score_texture = nullptr;


void GameEngine::generateRandomCoordinate(SDL_Rect &rect) const {
    rect.x = (rand() % game_window_width / snake_segment_size) * snake_segment_size;
    rect.y = (rand() % game_window_height / snake_segment_size) * snake_segment_size;
}

void GameEngine::validatePreyCoordinates(SDL_Rect &rect) {
    std::vector<SDL_Rect> snake_coords = snake->getBody();
    int snk_len = snake_coords.size();
    for (int i = 0; i < snk_len; i++)
        if (snake_coords[i].x == prey.x && snake_coords[i].y == prey.y) {
            generateRandomCoordinate(prey);
            i = 0;
        }

}

void GameEngine::init(const char *title, int xpos, int ypos, int w_width, int w_height, int gw_xpos, int gw_ypos,
                      int gw_width, int gw_height, bool fullscreen) {
    // ~~~~~~~~~~< Initialization >~~~~~~~~~
    window_width = w_width;
    window_height = w_height;
    game_window_width = gw_width;
    game_window_height = gw_height;
    game_window_x = gw_xpos;
    game_window_y = gw_ypos;
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    int flags = 0;

    if (fullscreen)
        flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        puts("Subsystems Initialized...");

        window = SDL_CreateWindow(title, xpos, ypos, w_width, w_height, flags);

        if (window)
            puts("Window created.");

        renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer) {
//            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
            puts("Renderer created.");
        }

        isRunning = true;

    } else {
        isRunning = false;
    }


    game_window_rect = {game_window_x, game_window_y, game_window_width, game_window_height};

    snake = new Snake(renderer, gw_xpos + 0, gw_ypos + (game_window_height / 2), snake_segment_size, snake_len, gw_xpos,
                      gw_ypos, gw_width, gw_height);

    prey.w = snake_segment_size;
    prey.h = snake_segment_size;

    srand(time(nullptr));
    validatePreyCoordinates(prey);

    // ~~~~~~~~~< Font Stuff >~~~~~~~~~~
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
//    blockway_font = TTF_OpenFont("Minecraft.ttf", 24);
//    blockway_font = TTF_OpenFont("joystix monospace.ttf", 24);
//    blockway_font = TTF_OpenFont("3X5_____.ttf", 24);
//    blockway_font = TTF_OpenFont("RetroscapeRegular-V5dZ.ttf", 24);
    blockway_font = TTF_OpenFont("BlockwayPixies-8K2D.ttf", 24);

    score_rect = {(game_window_width - 240) / 2, game_window_y + game_window_height + 20, 240, 50};

    char score[5];
    snprintf(score, 5, "%04d", snake->getLength());
    score_surface = TTF_RenderText_Solid(blockway_font, score, white);
//    score_surface = TTF_RenderText_Solid(blockway_font, std::to_string(snake_len).c_str(), white);
    score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}

void GameEngine::reinit() {
    delete snake;
    snake = new Snake(renderer, game_window_x + 0, game_window_y + (game_window_height / 2),
                      snake_segment_size, snake_len, game_window_x, game_window_y, game_window_width,
                      game_window_height);

    validatePreyCoordinates(prey);

    gameOver = false;
    snake_direction = 0;
}

void GameEngine::handleEvents() {
    // Event loop. Handles events on queue
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            isRunning = false;
        else if (event.type == SDL_KEYDOWN) {
            // Move based on keypress
            switch (event.key.keysym.sym) {
                case SDLK_UP:
//                    puts("up");
//                    snake->move(SDLK_UP);
                    if (snake->getCurrentDirection() == SDLK_DOWN)
                        snake_direction = SDLK_DOWN;
                    else
                        snake_direction = SDLK_UP;
                    break;

                case SDLK_DOWN:
//                    puts("down");
//                    snake->move(SDLK_DOWN);
                    if (snake->getCurrentDirection() == SDLK_UP)
                        snake_direction = SDLK_UP;
                    else
                        snake_direction = SDLK_DOWN;
                    break;

                case SDLK_LEFT:
//                    puts("left");
//                    snake->move(SDLK_LEFT);
                    if (snake->getCurrentDirection() == SDLK_RIGHT)
                        snake_direction = SDLK_RIGHT;
                    else
                        snake_direction = SDLK_LEFT;
                    break;

                case SDLK_RIGHT:
//                    puts("right");
//                    snake->move(SDLK_RIGHT);
                    if (snake->getCurrentDirection() == SDLK_LEFT)
                        snake_direction = SDLK_LEFT;
                    else
                        snake_direction = SDLK_RIGHT;
                    break;

                case SDLK_r:
                    reinit();
                    break;

                default:
                    break;
            }
        }

    }
}

void GameEngine::update() {
    // If snake is dead, stop snake
    if (!gameOver) {
        if (snake->isDead()) {
            gameOver = true;
            return;
        }

        snake->move(snake_direction);

        SDL_Rect head = snake->getHead();
        // If the snakes head and prey share the same coordinates
        if (head.x == prey.x && head.y == prey.y) {
            snake->grow();
            printf("Snake length: %d\n", snake->getLength());
            validatePreyCoordinates(prey);
        }

//        score_surface = TTF_RenderText_Solid(blockway_font,"▶", white);
//        score_surface = TTF_RenderText_Solid(blockway_font,"⏸", white);
        char score[5];
        snprintf(score, 5, "%04d", snake->getLength());
        score_surface = TTF_RenderText_Solid(blockway_font, score, white);
//        score_surface = TTF_RenderText_Solid(blockway_font, std::to_string(snake->getLength()).c_str(), white);
        score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);

        SDL_Delay(100);
    }
}

void GameEngine::render() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);

    // Add stuff to render here
    snake->render();

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDrawRect(renderer, &game_window_rect);

    SDL_RenderFillRect(renderer, &prey);

//    SDL_FreeSurface(score_surface);
    SDL_RenderCopy(renderer, score_texture, nullptr, &score_rect);
//    SDL_DestroyTexture(score_texture);

    // Present
    SDL_RenderPresent(renderer);
}

void GameEngine::clean() {
    delete snake;
    TTF_CloseFont(blockway_font);
    SDL_DestroyTexture(score_texture);
    SDL_FreeSurface(score_surface);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
    puts("Game cleaned.");
}

