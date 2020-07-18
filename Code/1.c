#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region HEAD
#pragma region DESCRIPTION

/* DEFINED PROGRESS GOALS
 * 
 * Introducing Game-States
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
int game_state;
enum game_state
{
	GS_PLAYING,
	GS_PAUSED,
	GS_IDLE,
	GS_OVER,
	GS_INTRO
};
int bird_move = 1;
#pragma region VISIBLES
SDL_Surface *temp_surface = NULL;
// Background
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

void game_state_check(void);
void gameSet(void);

void playing_set(void);
void playing_update(void);
void playing_draw(void);

void paused_set(void);
void paused_update(void);
void paused_draw(void);

void idle_set(void);
void idle_update(void);
void idle_draw(void);

void intro_set(void);
void intro_update(void);
void intro_draw(void);

void game_over_set(void);
void game_over_Update(void);
void game_over_Draw(void);
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
	int running = 1;
	SDL_SetRenderDrawColor(Renderer, WHITE);
	game_state = GS_IDLE;
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
		SDL_RenderClear(Renderer);
		game_state_check();
		SDL_RenderPresent(Renderer);
		SDL_Delay(25);
	}
#pragma endregion MAIN LOOP

	assets_out();
	exit_();
	return EXIT_SUCCESS;
}
#pragma endregion MAIN FUNCTION

#pragma region FUNCTIONS
void game_state_check(void)
{
	switch (game_state)
	{
	case GS_PLAYING:
		playing_update();
		break;

	case GS_PAUSED:
		paused_update();
		break;

	case GS_IDLE:
		idle_update();
		break;

	case GS_OVER:
		game_over_Update();
		break;

	default:
		break;
	}
}

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

void idle_set(void)
{
}

void idle_update(void)
{
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
	idle_draw();
}

void idle_draw(void)
{

	SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);
	SDL_RenderCopy(Renderer, atlas, &grd_src, &grd_dst);
	SDL_RenderCopy(Renderer, atlas, &bird_src, &bird_dst);
}

void intro_set(void)
{
}

void intro_update(void)
{
}

void intro_draw(void)
{
}

void playing_set(void)
{
}

void playing_update(void)
{
}

void playing_draw(void)
{
}

void paused_set(void)
{
}

void paused_update(void)
{
}

void paused_draw(void)
{
}

void game_over_set(void)
{
}

void game_over_Update(void)
{
}

void game_over_Draw(void)
{
}

void assets_out(void)
{
	SDL_DestroyTexture(atlas);
}
#pragma endregion FUNCTIONS
