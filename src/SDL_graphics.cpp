//Using SDL2
#include <SDL.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

SDL_Renderer *renderer;

bool running;

//Attempts to create SDL window, returns true if sucessful
bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Xhaust", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//create renderer from window
			renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	// gHelloWorld = SDL_LoadBMP("02_getting_an_image_on_the_screen/hello_world.bmp");
	// if (gHelloWorld == NULL)
	// {
	// 	printf("Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
	// 	success = false;
	// }

	return success;
}

void close()
{
	//Deallocate surface (already done).
	//SDL_FreeSurface( gHelloWorld );

	// gHelloWorld = NULL;

	// SDL_DestroyTexture(gHelloWorld_tex);
	// gHelloWorld_tex = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

Uint32 update(Uint32 dt)
{

	Uint32 startTime = SDL_GetTicks();

	// draw(dt);
	SDL_RenderPresent(renderer);

	//Update the surface
	//SDL_UpdateWindowSurface( gWindow );

	SDL_Event event;

	running = true;

	//Event Handling
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			printf("Key press detected\n");
			break;

		case SDL_KEYUP:
			printf("Key release detected\n");
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				printf("Window Close Event\n");
				running = false;
			}
			break;

		default:
			break;
		}
	}

	return startTime - SDL_GetTicks();
}

int main(int argc, char *args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
		return 1;
	}

	//Load media
	if (!loadMedia())
	{
		printf("Failed to load media!\n");
		return 2;
	}

	//Gameloop should go here:

	running = true;

	Uint32 dt = update(17);
	Uint32 t = 0;

	while (running)
	{
		dt = update(dt);

		SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
		SDL_RenderClear(renderer);

		t += dt;

		//TODO smooth anim
		SDL_Rect fillRect = {10 + 0 * t / 1000, 10, 100, 100}; //SCREEN_WIDTH / 4 + t, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(renderer, &fillRect);

		if (dt < 17)
		{
			//sleep any remaining time to make sure it's at least a 17ms delay
			SDL_Delay(17 - dt);
			dt = 17;
		}
	}

	//Free resources and close SDL
	close();
	return 0;
}