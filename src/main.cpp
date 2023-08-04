#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FPS 60
#define FRAME_DELAY (1000 / FPS)

#define RECT_HEIGHT 96
#define RECT_WIDTH 8
#define RECT_SPEED 3

#define BALL_SIZE 16
#define BALL_SPEED_X 3
#define BALL_SPEED_Y 2

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error: SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("CPPong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Log("Error: SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Error: SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Create player rectangle.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = {0, (WINDOW_HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);

    // Create ball rectangle.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect ball = {WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE};
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);

    SDL_RenderPresent(renderer);

    /* Event Loop Starts. */

    int direction = -1; // 1 = Downward, -1 = Upward
    int ballDirX = -1;  // 1 = Right, -1 = Left
    int ballDirY = -1;  // 1 = Downward, -1 = Upward

    SDL_Event event;
    int quit = 0;

    Uint32 frameStart;
    int frameTime;
    while (!quit)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;

            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP && direction == 1)
                {
                    direction = -1;
                    std::cout << "ARROW_UP" << std::endl;
                }
                else if (event.key.keysym.sym == SDLK_DOWN && direction == -1)
                {
                    direction = 1;
                    std::cout << "ARROW_DOWN" << std::endl;
                }
            }
        }

        // Let's make that fucking rect move.
        rect.y += direction * RECT_SPEED;

        // Movement limits.
        if (rect.y <= 0 || rect.y >= (WINDOW_HEIGHT - RECT_HEIGHT))
        {
            direction *= -1;
            rect.y += direction * RECT_SPEED;
        }

        // Movement of ball.
        ball.x += ballDirX * BALL_SPEED_X;
        ball.y += ballDirY * BALL_SPEED_Y;

        // Collision detection.
        // Ball can touch ceiling and floor of window when that happens change ball's y direction.
        // if ball touches right and left sides of window destroy ball, handle scores.

        // Collision with TOP and BOTTOM of window.
        if (ball.y <= 0 || ball.y >= (WINDOW_HEIGHT - BALL_SIZE))
        {
            ballDirY *= -1;

            // Recalculate Y value
            ball.y += ballDirY * BALL_SPEED_Y;
        }

        // Collision with player rectangle
        if (ball.x <= RECT_WIDTH)
        {
            ballDirX *= -1;

            // Recalculate X value
            ball.x += ballDirX * BALL_SPEED_X;
        }

        // Score handling.
        if (ball.x <= 0)
        {
            std::cout << "Player at right gained 1 point." << std::endl;

            // Set back to the defaults.
            rect.x = 0;
            rect.y = (WINDOW_HEIGHT - RECT_HEIGHT) / 2;
            direction = -1;

            ball.x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
            ball.y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
            ballDirY = -1;
            ballDirX = -1;
        }
        else if (ball.y >= (WINDOW_WIDTH - BALL_SIZE))
        {
            std::cout << "Player at left gained 1 point." << std::endl;

            // Set back to the defaults.
            rect.x = 0;
            rect.y = (WINDOW_HEIGHT - RECT_HEIGHT) / 2;
            direction = -1;

            ball.x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
            ball.y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
            ballDirY = -1;
            ballDirX = -1;
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw player rectangle.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Draw ball.
        SDL_RenderFillRect(renderer, &ball);

        // Update.
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);
    }

    /* Event Loop Ends. */

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}