#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region HEAD
#pragma region DESCRIPTION

/* DEFINED PROGRESS GOALS
 * 
 * Render random bird color and day/night background
 * 
 */
#pragma endregion DESCRIPTION

#pragma region INCLUDES
//system headers
#include <math.h>
//local headers
#include "helper.h"
#include <time.h>
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
//bg
SDL_Texture *atlas = NULL;
SDL_Rect bg_src;
SDL_Rect grd_src;
SDL_Rect grd_dst;
SDL_Rect bird_src;
SDL_Rect bird_dst;
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
	srand(time(NULL));
	assets_in();

#pragma region WINDOW
	SDL_SetWindowPosition(Window, 0, 0);
	SDL_SetWindowSize(Window, ww, wh);
	SDL_SetWindowTitle(Window, "flap, flap");
	SDL_ShowWindow(Window);
#pragma endregion WINDOW

	SDL_Event event;
	int bird_move = 1;
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

#pragma region LOGIC
		//move ground
		grd_src.x += 1;
		if (grd_src.x >= 48 + 584)
			grd_src.x = 584;

		//animate bird
		if (bird_src.x == 355)
			bird_src.x = 390;
		else if (bird_src.x == 390)
			bird_src.x = 425;
		else if (bird_src.x == 425)
			bird_src.x = 355;

		if (bird_move)
		{
			bird_dst.y++;
			if (bird_dst.y >= 250)
				bird_move = 0;
		}
		else
		{
			bird_dst.y--;
			if (bird_dst.y <= 200)
				bird_move = 1;
		}
#pragma endregion LOGIC

#pragma region RENDERING
		SDL_SetRenderDrawColor(Renderer, WHITE);
		SDL_RenderClear(Renderer);
		SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);
		SDL_RenderCopy(Renderer, atlas, &grd_src, &grd_dst);
		SDL_RenderCopy(Renderer, atlas, &bird_src, &bird_dst);
		SDL_RenderPresent(Renderer);
#pragma endregion RENDERING
		SDL_Delay(25);
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

	temp_surface = IMG_Load("../../assets/gfx/atlas3.png");
	atlas = SDL_CreateTextureFromSurface(Renderer, temp_surface);
	int bg = rand() % 2;
	if (bg)
	{
		bg_src.x = 0;
	}
	else
	{
		bg_src.x = 292;
	}
	bg_src.y = 0;
	bg_src.w = 288;
	bg_src.h = 512;

	grd_src.x = 584;
	grd_src.y = 0;
	grd_src.w = 288;
	grd_src.h = 112;

	grd_dst.x = 0;
	grd_dst.y = 400;
	grd_dst.w = 336;
	grd_dst.h = 112;

	bird_src.x = 355;
	bird_src.w = 34; //max 336
	bird_src.h = 24;
	int bird = rand() % 3;
	if (bird == 0)
		bird_src.y = 757;
	else if (bird == 1)
		bird_src.y = 783;
	else if (bird == 2)
		bird_src.y = 809;
	bird_src.w = 34;
	bird_src.h = 24;

	bird_dst.x = 75;
	bird_dst.y = 200;
	bird_dst.w = 34;
	bird_dst.h = 24;
}

void assets_out(void)
{
	SDL_DestroyTexture(atlas);
}

#pragma endregion FUNCTIONS
