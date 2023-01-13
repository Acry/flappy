#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region HEAD
#pragma region DESCRIPTION

/* This is a Flappy Bird Implementation
 *
 */

/* DEFINED PROGRESS GOALS
 * 
 * Create Intro State
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
SDL_Event event;
int running = 1;
#pragma region VISIBLES
SDL_Surface *temp_surface = NULL;
SDL_Texture *atlas = NULL;

SDL_Rect bg_src;
SDL_Rect grd_src;
SDL_Rect grd_dst;
SDL_Rect bird_src;
SDL_Rect bird_dst;

SDL_Rect title_src; // Title
SDL_Rect title_dst;
SDL_Rect play_src; // Play Button
SDL_Rect play_dst;
SDL_Rect lb_src; // Leaderboard
SDL_Rect lb_dst;
SDL_Rect cr_src; // Copyright
SDL_Rect cr_dst;
SDL_Rect rate_src; // Rate Button
SDL_Rect rate_dst;

SDL_Rect ready_src;
SDL_Rect ready_dst;
SDL_Rect intro_src;
SDL_Rect intro_dst;
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

void update_ground(void);
void update_bird(void);
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
	idle_set();
	intro_set();

#pragma region WINDOW
	SDL_SetWindowPosition(Window, 0, 0);
	SDL_SetWindowSize(Window, ww, wh);
	SDL_SetWindowTitle(Window, "flap, flap");
	SDL_ShowWindow(Window);
#pragma endregion WINDOW

	SDL_SetRenderDrawColor(Renderer, WHITE);
	// game_state = GS_IDLE;
	game_state = GS_INTRO;
#pragma endregion INIT

#pragma region MAIN LOOP
	while (running)
	{
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

	case GS_INTRO:
		intro_update();
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

	temp_surface = IMG_Load("../../assets/gfx/atlas2.png");
	atlas = SDL_CreateTextureFromSurface(Renderer, temp_surface);
}

void idle_set(void)
{

	// BG
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

	// Ground
	grd_src.x = 584;
	grd_src.y = 0;
	grd_src.w = 288;
	grd_src.h = 112;

	grd_dst.x = 0;
	grd_dst.y = 400;
	grd_dst.w = 336;
	grd_dst.h = 112;

	// Bird
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

	bird_dst.h = 24;
	bird_dst.w = 34;
	bird_dst.x = (ww / 2) - (bird_dst.w / 2);
	bird_dst.y = (wh / 2) - (bird_dst.h);

	// Play button
	play_src.x = 708;
	play_src.y = 234;
	play_src.w = 104;
	play_src.h = 59;

	play_dst.w = play_src.w;
	play_dst.h = play_src.h;
	play_dst.x = 20;
	play_dst.y = grd_dst.y - play_dst.h;

	// Leaderboard button
	lb_src.x = 828;
	lb_src.y = 236;
	lb_src.w = 104;
	lb_src.h = 58;

	lb_dst.w = play_src.w;
	lb_dst.h = play_src.h;
	lb_dst.x = play_dst.x + 30 + play_dst.w;
	lb_dst.y = grd_dst.y - lb_dst.h + 1;

	// Rate button
	rate_src.x = 930;
	rate_src.y = 2;
	rate_src.w = 62;
	rate_src.h = 36;

	rate_dst.w = rate_src.w;
	rate_dst.h = rate_src.h;
	rate_dst.x = (ww / 2) - (rate_src.w / 2);
	;
	rate_dst.y = 280;

	// Copyright
	cr_src.x = 886;
	cr_src.y = 184;
	cr_src.w = 122;
	cr_src.h = 10;

	cr_dst.w = cr_src.w;
	cr_dst.h = cr_src.h;
	cr_dst.x = (ww / 2) - (cr_dst.w / 2);
	cr_dst.y = grd_dst.y + 20;

	// Title
	title_src.x = 702;
	title_src.y = 182;
	title_src.w = 178;
	title_src.h = 48;

	title_dst.w = 178;
	title_dst.h = 48;
	title_dst.x = (ww / 2) - (title_dst.w / 2);
	title_dst.y = 150;
}

void idle_update(void)
{
	SDL_GetMouseState(&mouse.x, &mouse.y);
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			running = 0;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_Log("Button Pressed");
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_Log("Left Button");
				if (SDL_PointInRect(&mouse, &play_dst))
				{
					SDL_Log("Hit");
					break;
					intro_set();
					game_state = GS_INTRO;
				}
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				running = 0;
			}
		}
	}
	update_ground();
	update_bird();
	idle_draw();
}

void idle_draw(void)
{

	SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);
	SDL_RenderCopy(Renderer, atlas, &grd_src, &grd_dst);

	SDL_RenderCopy(Renderer, atlas, &title_src, &title_dst);
	SDL_RenderCopy(Renderer, atlas, &play_src, &play_dst);
	SDL_RenderCopy(Renderer, atlas, &lb_src, &lb_dst);

	SDL_RenderCopy(Renderer, atlas, &bird_src, &bird_dst);

	SDL_RenderCopy(Renderer, atlas, &cr_src, &cr_dst);
	SDL_RenderCopy(Renderer, atlas, &rate_src, &rate_dst);
}

void intro_set(void)
{

	ready_src.w = 184;
	ready_src.h = 50;
	ready_src.x = 590;
	ready_src.y = 118;

	ready_dst.w = ready_src.w;
	ready_dst.h = ready_src.h;
	ready_dst.x = (ww / 2) - (ready_dst.w / 2);
	ready_dst.y = 130;

	intro_src.w = 114;
	intro_src.h = 98;
	intro_src.x = 585;
	intro_src.y = 182;

	intro_dst.w = intro_src.w;
	intro_dst.h = intro_src.h;
	intro_dst.x = (ww / 2) - (intro_dst.w / 2);
	intro_dst.y = 220;

	bird_dst.x -= 60;
}

void intro_update(void)
{

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			running = 0;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				break;
				playing_set();
				game_state = GS_PLAYING;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				running = 0;
			}
		}
	}

	update_ground();
	update_bird();
	intro_draw();
}

void intro_draw(void)
{

	SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);
	SDL_RenderCopy(Renderer, atlas, &grd_src, &grd_dst);
	SDL_RenderCopy(Renderer, atlas, &intro_src, &intro_dst);
	SDL_RenderCopy(Renderer, atlas, &ready_src, &ready_dst);
	SDL_RenderCopy(Renderer, atlas, &bird_src, &bird_dst);
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

void update_ground(void)
{

	grd_src.x += 1;
	if (grd_src.x >= 48 + 584)
		grd_src.x = 584;
}

void update_bird(void)
{

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
}

void assets_out(void)
{

	SDL_DestroyTexture(atlas);
}

#pragma endregion FUNCTIONS
