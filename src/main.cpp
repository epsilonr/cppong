#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <iostream>
#include <ctime>

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

int getRandomDirection()
{
    return (std::rand() % 2) ? -1 : 1;
}
int main(int argc, char *argv[])
{
    std::srand(std::time(0));

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

    // Create AI rectangle.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect airect = {WINDOW_WIDTH - RECT_WIDTH, (WINDOW_HEIGHT - RECT_HEIGHT) / 2, RECT_WIDTH, RECT_HEIGHT};
    SDL_RenderFillRect(renderer, &airect);

    // Create ball rectangle.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect ball = {WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE};
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);

    SDL_RenderPresent(renderer);

    //! I should have used classes...
    //! But I am too lazy to change.
    int direction = getRandomDirection(); // 1 = Downward, -1 = Upward
    int ballDirX = getRandomDirection();  // 1 = Right, -1 = Left
    int ballDirY = getRandomDirection();  // 1 = Downward, -1 = Upward
    int estimatedBallY = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;

    /* Event Loop Starts. */
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
                    direction = -1;
                else if (event.key.keysym.sym == SDLK_DOWN && direction == -1)
                    direction = 1;
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
        if (ball.x <= RECT_WIDTH && rect.y <= ball.y && ball.y <= rect.y + RECT_HEIGHT)
        {
            ballDirX *= -1;

            // Recalculate X value
            ball.x += ballDirX * BALL_SPEED_X;
        }

        // Create invincible AI
        estimatedBallY = ball.y + BALL_SIZE / 2;

        // estimatedBallY = ball.y + BALL_SIZE / 2 - RECT_HEIGHT / 2;

        if (!(airect.y <= 0 || airect.y >= WINDOW_HEIGHT - RECT_HEIGHT) || !(estimatedBallY <= (RECT_HEIGHT / 2) || estimatedBallY >= (WINDOW_HEIGHT - RECT_HEIGHT / 2)))
            airect.y = estimatedBallY - RECT_HEIGHT / 2;

        // Collision with AI rectangle
        if (ball.x >= (WINDOW_WIDTH - RECT_WIDTH * 2) && airect.y <= ball.y && ball.y <= airect.y + RECT_HEIGHT)
        {
            ballDirX *= -1;

            // Recalculate X value
            ball.x += ballDirX * BALL_SPEED_X;
        }

        // Score handling.
        if (ball.x <= 0)
        {
            std::cout << "Player at right side gained 1 point." << std::endl;

            // Set back to the defaults.
            airect.x = WINDOW_WIDTH - RECT_WIDTH;
            airect.y = (WINDOW_HEIGHT - RECT_HEIGHT) / 2;

            rect.x = 0;
            rect.y = (WINDOW_HEIGHT - RECT_HEIGHT) / 2;
            direction = getRandomDirection();

            ball.x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
            ball.y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
            ballDirY = getRandomDirection();
            ballDirX = getRandomDirection();
        }
        else if (ball.x >= WINDOW_WIDTH)
        {
            std::cout << "Player at left side gained 1 point." << std::endl;

            // Set back to the defaults.
            airect.x = WINDOW_WIDTH - RECT_WIDTH;
            airect.y = (WINDOW_HEIGHT - RECT_HEIGHT) / 2;

            rect.x = 0;
            rect.y = (WINDOW_HEIGHT - RECT_HEIGHT) / 2;
            direction = getRandomDirection();

            ball.x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
            ball.y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
            ballDirY = getRandomDirection();
            ballDirX = getRandomDirection();
        }

        // Clear screen.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw player rectangle.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);

        // Draw AI rectangle.
        SDL_RenderFillRect(renderer, &airect);

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