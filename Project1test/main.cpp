#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int windowWidth = 640;
int windowHeight = 480;
Uint64 lastTime = 0;


class Player {
private:
    SDL_FRect rect;
    float width;
    float height;
    float speed;
    float positionX;
    float positionY;

public:
    Player(float w, float h, float startX, float startY) {
        width = w;
        height = h;
        positionX = startX;
        positionY = startY;
        rect.x = positionX;
        rect.y = positionY;
        rect.w = width;
        rect.h = height;
        speed = 200.0f;
    }

    void movePlayer(float dX, float dY, float dT) {
        positionY += dY* speed * dT;  // Move player up
		positionX += dX * speed * dT;  // Move player up
        rect.y = positionY;  // Update rectangle position
		rect.x = positionX;  // Update rectangle position    
        
    }

    int calcConsistentSpeed(float dX, float dY) {
        int hypotenuse = sqrt(dX * dX + dY * dY);
        return hypotenuse;
    }

    
    int playerBorderCollisionChecker() {
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
           
        SDL_Log("Height: %d Width: %d", windowHeight, windowWidth);

    };

    void update();

    void render(SDL_Renderer* renderer) {
        //Färg på spelaren
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        //Renderar spelaren som en rektangel
        SDL_RenderFillRect(renderer, &rect);
    };


};

static Player playerOne(100, 100, 50, 50);


//Runs at Startup
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    

    //Sätter Metadata för appen
    SDL_SetAppMetadata("SPEL", "1.0", "2d Game");

    //If SDL_Init() fails, it returns a negative error code. SDL_GetError() returns a string with the error message.
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    
    
    //Kan Force:a storlek
    //Create a window and renderer. The window is resizable, and the renderer will be used to draw into the window.
    if (!SDL_CreateWindowAndRenderer("Spel", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    return SDL_APP_CONTINUE;  /* carry on with the program! */



};




//Runs when the user does something, like click a button or move the mouse
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }



    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

//Runs every frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
	// Get the current time in seconds since the program started, and use it to compute a color that changes over time.
    


    Uint64 currentTime = SDL_GetTicks();

    //
    float dT = (currentTime - lastTime) / 1000.0f;
    //Resettar för denna frame
    lastTime = currentTime;

    const bool* keystate = SDL_GetKeyboardState(NULL);
    int moveX = 0; 
    int moveY = 0;


    if (keystate[SDL_SCANCODE_W]) {
        moveY = -1;
    }
    if (keystate[SDL_SCANCODE_A]) {
        moveX = -1;  // Vänster
    }
    if (keystate[SDL_SCANCODE_S]) {
        moveY = 1;   // Ner
    }
    if (keystate[SDL_SCANCODE_D]) {
        moveX = 1;   // Höger
    }
    int hypotenuse = playerOne.calcConsistentSpeed(moveX, moveY);

    playerOne.movePlayer(moveX/hypotenuse, moveY/hypotenuse, dT);


    SDL_SetRenderDrawColor(renderer, 100, 149, 237, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    
    

    
    playerOne.render(renderer);  /* render the player. */

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_Log("Game Is Shutting Down");
    /* SDL will clean up the window/renderer for us. */
}