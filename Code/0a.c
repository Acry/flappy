#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region HEAD
#pragma region DESCRIPTION

/* This is a light SDL2 template
 *
 */

/* DEFINED PROGRESS GOALS
 * 
 * render game icon & background
 * 
 */
#pragma endregion DESCRIPTION

#pragma region INCLUDES
//system headers
#include <math.h>
//local headers
#include "helper.h"
#pragma endregion INCLUDES

#pragma region CPP DEFINITIONS
#define WHITE 255, 255, 255, 255
#define BLACK 0, 0, 0, 255
#define RED 255, 0, 0, 255
#define WW 288
#define WH 512
#pragma endregion CPP DEFINITIONS

#pragma region DATASTRUCTURES
#pragma endregion DATASTRUCTURES

#pragma region GLOBALS
int ww = WW;
int wh = WH;

#pragma region VISIBLES
SDL_Surface *temp_surface = NULL;
// Background
SDL_Texture *atlas = NULL;
SDL_Rect bg_src;
#pragma endregion VISIBLES

SDL_Point mouse;

#pragma endregion GLOBALS

#pragma region FUNCTION PROTOTYPES
void assets_in(void);
void assets_out(void);
#pragma endregion FUNCTION PROTOTYPES

#pragma endregion HEAD

#pragma region MAIN FUNCTION
int main(int argc, char *argv[])
{

	(void)argc;
	(void)argv;

#pragma region INIT
	init();
	assets_in();

#pragma region WINDOW
	SDL_SetWindowPosition(Window, 0, 0);
	SDL_SetWindowSize(Window, ww, wh);
	SDL_SetWindowTitle(Window, "flap, flap");
	SDL_ShowWindow(Window);
#pragma endregion WINDOW

	SDL_Event event;
	int running = 1;
#pragma endregion INIT

#pragma region MAIN LOOP
	while (running)
	{

#pragma region EVENT LOOP
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = 0;
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					;
				}
				if (event.button.button == SDL_BUTTON_MIDDLE)
				{
					;
				}
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					;
				}
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = 0;
					break;

				case SDLK_r:
				case SDLK_BACKSPACE:
					break;

				case SDLK_p:
				case SDLK_SPACE:
					break;

				default:
					break;
				}
			}
		}
#pragma endregion EVENT LOOP
#pragma region RENDERING
		SDL_SetRenderDrawColor(Renderer, WHITE);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);
		SDL_RenderPresent(Renderer);
#pragma endregion RENDERING
	}
#pragma endregion MAIN LOOP

	assets_out();
	exit_();
	return EXIT_SUCCESS;
}
#pragma endregion MAIN FUNCTION

#pragma region FUNCTIONS
void assets_in(void)
{

#pragma region BG
	temp_surface = IMG_Load("../../assets/gfx/atlas.png");
	atlas = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	bg_src.x = 0;
	bg_src.y = 0;
	bg_src.w = 288;
	bg_src.h = 512;
#pragma endregion BG
}

void assets_out(void)
{
	SDL_DestroyTexture(atlas);
}

#pragma endregion FUNCTIONS
