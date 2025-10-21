#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include "entity.h"
#include "universe.h"
#include <fstream>  

using namespace std;

int windowWidth = 800;
int windowHeight = 600;

// load global variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

Universe universe;

Entity sun("star", "Sun", "Star", 
            10000, 1.0, 100,  // large mass
            0, 0, 0, 
            0, 0);

Entity earth("planet", "Earth", "Planet", 
             50, 1.0, 30, 
             300, 0, 0, 
             0, 2); // give initial Y velocity

SDL_Texture* lemon = nullptr;

// SDL functions
void initSDL() 
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    SDL_CreateWindowAndRenderer(windowWidth, windowHeight, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Universe simulation");
}

void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, int width, int height, double angle) 
{
    // create recrectangle
    SDL_Rect destRect;
    destRect.x = x - width / 2; // center the texture
    destRect.y = y - height / 2; // center the texture
    destRect.w = width;
    destRect.h = height;

    // rotate around center
    SDL_Point center = {destRect.w / 2, destRect.h / 2};

    // Render the texture to the specified rectangle
    SDL_RenderCopyEx(renderer, texture, NULL, &destRect, angle, &center, SDL_FLIP_NONE);
}

void drawEntity(SDL_Renderer* renderer, Entity& entity) 
{
    // draw entity using scaled position
    int screenX = windowWidth / 2 + entity.posX;
    int screenY = windowHeight / 2 + entity.posY;
    int size = entity.volume;
    drawTexture(renderer, lemon, screenX, screenY, size, size, entity.rotation);
}

void drawUniverse(SDL_Renderer* renderer, Universe& universe) 
{
    for (auto& entity : universe.entities) 
    {
        drawEntity(renderer, entity);
    }
}

int SDL_main(int argc, char* argv[])
{
    // initialize SDL
    initSDL();

    // load textures
    lemon = IMG_LoadTexture(renderer, "images/lemon.png");

    // set universe constants
    universe.setConstants(
        1.0f, // G
        5,         // epsilon
        5,           // N
        1, 1,    // dx, dy
        1,         // scale
        5,         // k (velocity cap scale)
        1         // friction (1% decay per frame)
    );

    // add entities
    universe.addEntity(sun);
    universe.addEntity(earth);

    // MAIN LOOP
    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // clear the window to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // render the current frame
        drawUniverse(renderer, universe);
        SDL_RenderPresent(renderer);
        
        // update universe state
        universe.update();
        
        // Delay to control frame rate
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}