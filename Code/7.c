#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma region HEAD
#pragma region DESCRIPTION
/* This is a Flappy Bird Implementation
 *
 */

/* DEFINED PROGRESS GOALS
 * 
 * Animations: Game Over Text
 * 
 */
#pragma endregion DESCRIPTION

#pragma region INCLUDES
//system headers
#include <math.h>
//local headers
#include "helper_1.h"
#include <time.h>
#pragma endregion INCLUDES

#pragma region CPP DEFINITIONS
#define WHITE 255, 255, 255, 255
#define BLACK 0, 0, 0, 255
#define RED 255, 0, 0, 255
#define WW 288
#define WH 512

#define MIX_VOLUME 90 // max 128

// Pipes
#define PIPES 6
#define PIPE_START 600 // Where a pipe spawns on the x-axis
#define PIPE_OPEN 100

#define PIPE_MIN_Y 210 // constrains the minimum top of a pipe
#define PIPE_MAX_Y 370 // constrains the maximum top of a pipe

#define PIPE_X_OFFSET 180 // offset between pipes
#define PIPE_STEP 2
#pragma endregion CPP DEFINITIONS

#pragma region DATASTRUCTURES
#pragma endregion DATASTRUCTURES

#pragma region GLOBALS
#pragma region common
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

char running = 1;

char flap_state = 0;
int flap_counter = 0;
int flap_delay = 17;

char bird_flip_direction = 0;
double bird_angle;
double vy = 12;
double vx = 200;
double gy = 800;

Uint32 FPS = 60;

// Pipes
SDL_Rect pipe_src;		  // need one source rectangle for the pipes
SDL_Rect pipe_dst[PIPES]; // but as many dest-rests as max. shown

// Score
SDL_Rect number_src[10];
SDL_Rect number_dst[10];
int digits[10];
Uint16 current_score;
int highest_score;
SDL_bool pipe_passed_player[3];

// SOUNDS
Mix_Chunk *collide_chunk = NULL;
Mix_Chunk *flap_chunk = NULL;
Mix_Chunk *hit_chunk = NULL; // collision
Mix_Chunk *point_chunk = NULL;
Mix_Chunk *wooosh_chunk = NULL;

// TRANSITION
char flash;
char show_ready;
char fade_from_black;
char fade_to_black;

enum direction
{
	UP,
	DOWN
};
#pragma endregion common

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
SDL_Rect pause_src; // Pause
SDL_Rect pause_dst;

SDL_Rect ready_src;
SDL_Rect ready_dst;
SDL_Rect intro_src;
SDL_Rect intro_dst;

// Game Over
SDL_Rect over_src; // Game Over Word
SDL_Rect over_dst; // Game Over Word
int over_dst_final_y;
int over_dst_top_y;
char over_animation;
enum direction over_animation_direction;

SDL_Rect sum_src;  // Summary Board
SDL_Rect sum_dst;  // Summary Board

SDL_Rect score_to_board_src[10];
SDL_Rect score_to_board_dst[3];
SDL_Rect hiscore_to_board_dst[3];
SDL_Rect new_hiscore_src;
SDL_Rect new_hiscore_dst;

// Medals
SDL_Rect medal_src;
SDL_Rect medal_dst;

SDL_Rect transition_rect;
Uint8 alpha;
Uint8 ready_alpha;
Uint8 fade_from_black_alpha;
Uint8 fade_to_black_alpha;
#pragma endregion VISIBLES

#pragma region INPUT
SDL_Event event;
SDL_Point mouse;
#pragma endregion

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
void update_pipes(void);

void bird_dies(void);
void bird_flap(void);
void bird_slide(void);
void bird_update(void);

void render_score(void);
void render_score_to_board(void);
void render_hiscore_to_board(void);
void read_highscore(void);
void write_highscore(void);
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
	playing_set();
	paused_set();
	game_over_set();
	read_highscore();
#pragma region WINDOW
	SDL_SetWindowPosition(Window, 0, 0);
	SDL_SetWindowSize(Window, ww, wh);
	SDL_SetWindowTitle(Window, "7 - Animations");
	SDL_ShowWindow(Window);
#pragma endregion WINDOW

	SDL_SetRenderDrawColor(Renderer, WHITE);
	game_state = GS_IDLE;
#pragma endregion INIT

#pragma region MAIN LOOP
	while (running)
	{
		game_state_check();
		// SDL_RenderClear(Renderer);
		SDL_RenderPresent(Renderer);

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
	flap_chunk = Mix_LoadWAV("../../assets/sfx/sfx_wing.ogg"); // flapping
	Mix_VolumeChunk(flap_chunk, MIX_VOLUME - 40);
	hit_chunk = Mix_LoadWAV("../../assets/sfx/sfx_hit.ogg"); // collide
	Mix_VolumeChunk(hit_chunk, MIX_VOLUME);
	wooosh_chunk = Mix_LoadWAV("../../assets/sfx/sfx_swooshing.ogg"); // state switch
	Mix_VolumeChunk(wooosh_chunk, MIX_VOLUME);
	collide_chunk = Mix_LoadWAV("../../assets/sfx/sfx_die.ogg");
	Mix_VolumeChunk(collide_chunk, MIX_VOLUME);
	point_chunk = Mix_LoadWAV("../../assets/sfx/sfx_point.ogg");
	Mix_VolumeChunk(point_chunk, MIX_VOLUME);

	temp_surface = IMG_Load("../../assets/gfx/atlas2.png");
	atlas = SDL_CreateTextureFromSurface(Renderer, temp_surface);

	transition_rect.h = wh;
	transition_rect.w = ww;
	transition_rect.x = 0;
	transition_rect.y = 0;
}

#pragma region idle
void idle_set(void)
{
	fade_from_black = 1;
	fade_from_black_alpha = 255;
	fade_to_black = 0;
	fade_to_black_alpha = 0;

	// Select BG
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

	// Select Bird
	int bird = rand() % 3;
	if (bird == 0)
		bird_src.y = 757;
	else if (bird == 1)
		bird_src.y = 783;
	else if (bird == 2)
		bird_src.y = 809;
	bird_src.x = 355;
	bird_src.w = 34; //max 336
	bird_src.h = 24;
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

	current_score = 0;
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (SDL_PointInRect(&mouse, &play_dst))
				{	
					fade_to_black = 1;
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
	if (fade_from_black){
		fade_from_black_alpha -= 5;
		if (fade_from_black_alpha == 0) fade_from_black = 0;
	}
	if (fade_to_black){
		fade_to_black_alpha += 5;
		if (fade_to_black_alpha == 255){
			fade_to_black = 0;
			intro_set();
			game_state = GS_INTRO;
		}
	}
	update_ground();
	bird_slide();
	bird_flap();
	idle_draw();
}

void idle_draw(void)
{
	SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);
	SDL_RenderCopy(Renderer, atlas, &grd_src, &grd_dst);

	SDL_RenderCopy(Renderer, atlas, &title_src, &title_dst);

	SDL_RenderCopy(Renderer, atlas, &play_src, &play_dst);
	SDL_RenderCopy(Renderer, atlas, &lb_src, &lb_dst);
	SDL_RenderCopy(Renderer, atlas, &rate_src, &rate_dst);

	SDL_RenderCopy(Renderer, atlas, &bird_src, &bird_dst);

	SDL_RenderCopy(Renderer, atlas, &cr_src, &cr_dst);

	if (fade_from_black)
	{
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, fade_from_black_alpha);
		SDL_RenderFillRect(Renderer, &transition_rect);
	}
	if (fade_to_black)
	{
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, fade_to_black_alpha);
		SDL_RenderFillRect(Renderer, &transition_rect);
	}
}
#pragma endregion idle

#pragma region intro
void intro_set(void)
{
	fade_from_black = 1;
	fade_from_black_alpha = 255;
	// Get Ready
	ready_src.w = 184;
	ready_src.h = 50;
	ready_src.x = 590;
	ready_src.y = 118;

	ready_dst.w = ready_src.w;
	ready_dst.h = ready_src.h;
	ready_dst.x = (ww / 2) - (ready_dst.w / 2);
	ready_dst.y = 130;

	// mini manual
	intro_src.w = 114;
	intro_src.h = 98;
	intro_src.x = 585;
	intro_src.y = 182;

	intro_dst.w = intro_src.w;
	intro_dst.h = intro_src.h;
	intro_dst.x = (ww / 2) - (intro_dst.w / 2);
	intro_dst.y = 220;

	// bird playing position
	bird_dst.x -= 60;

	// score
	int i;
	// init numbers source rects
	for (i = 0; i < 10; i++)
	{
		number_src[i].x = 358 + (i * (24 + 4));
		number_src[i].y = 691;
		number_src[i].w = 24;
		number_src[i].h = 36;
	}

	// init rendered score digits

	number_dst[0].w = 24;
	number_dst[0].h = 36;
	number_dst[0].y = 50;
	number_dst[0].x = ww / 2 - number_dst[0].w / 2;

	number_dst[1].w = 24;
	number_dst[1].h = 36;
	number_dst[1].y = 50;

	number_dst[2].w = 24;
	number_dst[2].h = 36;
	number_dst[2].y = 50;
	Mix_PlayChannel(-1, wooosh_chunk, 0);
}

void intro_update(void)
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				Mix_PlayChannel(-1, flap_chunk, 0);
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
	if (fade_from_black){
		fade_from_black_alpha -= 5;
		if (fade_from_black_alpha == 0) fade_from_black = 0;
	}
	update_ground();
	bird_slide();
	bird_flap();
	intro_draw();
}

void intro_draw(void)
{
	SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);
	SDL_RenderCopy(Renderer, atlas, &grd_src, &grd_dst);
	SDL_RenderCopy(Renderer, atlas, &intro_src, &intro_dst);
	SDL_RenderCopy(Renderer, atlas, &ready_src, &ready_dst);
	SDL_RenderCopy(Renderer, atlas, &bird_src, &bird_dst);
	render_score();
	if (fade_from_black)
	{
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, fade_from_black_alpha);
		SDL_RenderFillRect(Renderer, &transition_rect);
	}
}
#pragma endregion intro

#pragma region playing
void playing_set(void)
{
	// white flash
	alpha = 0;

	// get ready and mini instructions
	show_ready = 1;
	ready_alpha = 255;
#pragma region pipes
	// select day or night pipe by background
	if (bg_src.x == 0)
	{
		pipe_src.w = 52;
		pipe_src.h = 320;
		pipe_src.x = 168;
		pipe_src.y = 646;
	}
	else
	{
		pipe_src.w = 52;
		pipe_src.h = 320;
		pipe_src.x = 56;
		pipe_src.y = 646;
	}

	// Set pipe dest rects
	int i;
	for (i = 0; i < PIPES; i++)
	{
		pipe_dst[i].w = pipe_src.w;
		pipe_dst[i].h = pipe_src.h;
	}

	// Set pipe x pos. and opening (y)
	pipe_dst[0].x = PIPE_START;
	pipe_dst[0].y = rand() % (PIPE_MAX_Y + 1 - PIPE_MIN_Y) + PIPE_MIN_Y;
	pipe_dst[1].x = PIPE_START;
	pipe_dst[1].y = pipe_dst[0].y - (pipe_src.h + PIPE_OPEN);
	pipe_passed_player[0] = SDL_FALSE;

	pipe_dst[2].x = PIPE_START + PIPE_X_OFFSET;
	pipe_dst[2].y = rand() % (PIPE_MAX_Y + 1 - PIPE_MIN_Y) + PIPE_MIN_Y;
	pipe_dst[3].x = PIPE_START + PIPE_X_OFFSET;
	pipe_dst[3].y = pipe_dst[2].y - (pipe_src.h + PIPE_OPEN);
	pipe_passed_player[1] = SDL_FALSE;

	pipe_dst[4].x = PIPE_START + PIPE_X_OFFSET * 2;
	pipe_dst[4].y = rand() % (PIPE_MAX_Y + 1 - PIPE_MIN_Y) + PIPE_MIN_Y;
	pipe_dst[5].x = PIPE_START + PIPE_X_OFFSET * 2;
	pipe_dst[5].y = pipe_dst[4].y - (pipe_src.h + PIPE_OPEN);
	pipe_passed_player[2] = SDL_FALSE;

	vy = 0;
#pragma endregion
}

void playing_update(void)
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (SDL_PointInRect(&mouse, &pause_dst))
				{
					game_state = GS_PAUSED;
					break;
				}
				vy = -240;
				Mix_PlayChannel(-1, flap_chunk, 0);
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				running = 0;
			}
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				vy = -240;
				Mix_PlayChannel(-1, flap_chunk, 0);
				break;
			case SDLK_ESCAPE:
				running = 0;
				break;
			case SDLK_p:
				game_state = GS_PAUSED;
				break;
			case SDLK_g:
			case SDLK_r:
				idle_set();
				intro_set();
				playing_set();
				game_state = GS_IDLE;
				break;
			default:
				break;
			}
		}
	}
	if (show_ready){
		ready_alpha -= 5;
		if (ready_alpha == 0) show_ready = 0;
	}
	update_pipes();
	update_ground();
	bird_update();
	bird_flap();
	playing_draw();
}

void playing_draw(void)
{
	// background
	SDL_RenderCopy(Renderer, atlas, &bg_src, NULL);

	// bird
	SDL_RenderCopyEx(Renderer, atlas, &bird_src, &bird_dst, bird_angle, NULL, 0);

	// render pipes
	int i;
	for (i = 0; i < PIPES; i++)
	{
		if (i % 2)
			SDL_RenderCopyEx(Renderer, atlas, &pipe_src, &pipe_dst[i], 0, NULL, SDL_FLIP_VERTICAL);
		else
			SDL_RenderCopy(Renderer, atlas, &pipe_src, &pipe_dst[i]);
	}

	// ground
	SDL_RenderCopy(Renderer, atlas, &grd_src, &grd_dst);

	if (show_ready){

		SDL_SetTextureAlphaMod(atlas, ready_alpha);

		// get ready
		SDL_RenderCopy(Renderer, atlas, &ready_src, &ready_dst);

		// mini instructions
		SDL_RenderCopy(Renderer, atlas, &intro_src, &intro_dst);
		
		SDL_SetTextureAlphaMod(atlas, 255);
	}

	// pause sign
	SDL_RenderCopy(Renderer, atlas, &pause_src, &pause_dst);

	render_score();

	// white flash on death
	if (flash)
	{
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, alpha);
		SDL_RenderFillRect(Renderer, &transition_rect);
	}
}
#pragma endregion playing

#pragma region paused
void paused_set(void)
{
	pause_src.w = 26;
	pause_src.h = 28;
	pause_src.x = 242;
	pause_src.y = 612;

	pause_dst.w = pause_src.w;
	pause_dst.h = pause_src.h;
	pause_dst.x = 10;
	pause_dst.y = 430;
}

void paused_update(void)
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				game_state = GS_PLAYING;
			}
		}
		if (event.type == SDL_JOYBUTTONDOWN)
		{
			game_state = GS_PLAYING;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				running = 0;
				break;
			case SDLK_SPACE:
				game_state = GS_PLAYING;
				break;
			case SDLK_g:
			case SDLK_r:
				idle_set();
				intro_set();
				game_state = GS_IDLE;
				break;
			default:
				break;
			}
		}
	}
	paused_draw();
}

void paused_draw(void)
{
	//TBD: render paused image, maybe greyout screen
	playing_draw();
}
#pragma endregion paused

#pragma region game over
void game_over_set(void)
{
	fade_to_black_alpha = 0;
	// Game over literals
	over_src.w = 197;
	over_src.h = 52;
	over_src.x = 790;
	over_src.y = 118;


	over_dst_top_y = 120;
	over_dst_final_y = 130;
	over_dst.w = over_src.w;
	over_dst.h = over_src.h;
	over_dst.x = (ww / 2) - (over_dst.w / 2);
	over_dst.y = over_dst_final_y;

	over_animation = 1;
	over_animation_direction = UP;

	// Summary rects
	sum_src.w = 231;
	sum_src.h = 123;
	sum_src.x = 6;
	sum_src.y = 518;

	sum_dst.w = sum_src.w;
	sum_dst.h = sum_src.h;
	sum_dst.x = (ww / 2) - (sum_dst.w / 2);
	sum_dst.y = 185;

	// init medal source rect
	// set src.x on on score
	if (current_score > 9 && current_score < 19)
		medal_src.x = 309;
	else if (current_score > 19 && current_score < 29)
		medal_src.x = 309 + 1 * 46;
	else if (current_score > 29 && current_score < 39)
		medal_src.x = 309 + 2 * 46;
	else if (current_score > 39)
		medal_src.x = 309 + 3 * 46;
	else
		medal_src.x = 0;

	medal_src.y = 852;
	medal_src.w = 44;
	medal_src.h = 44;

	// init medal dest rect
	medal_dst.x = 57;
	medal_dst.y = 226;
	medal_dst.w = 44;
	medal_dst.h = 44;

	// score to board
	int i;

	// init medium sized digits source rects
	for (i = 0; i < 10; i++)
	{
		score_to_board_src[i].w = 14;
		score_to_board_src[i].h = 20;
		score_to_board_src[i].x = 317;
		score_to_board_src[i].y = 523 + (i * (score_to_board_src[i].h + 1));
	}

	// init medium sized digits dest rects
	score_to_board_dst[0].x = 222;
	score_to_board_dst[0].y = 214;
	score_to_board_dst[0].w = 14;
	score_to_board_dst[0].h = 20;

	score_to_board_dst[1].w = 14;
	score_to_board_dst[1].h = 20;
	score_to_board_dst[1].x = 0;
	score_to_board_dst[1].y = 214;

	score_to_board_dst[2].w = 14;
	score_to_board_dst[2].h = 20;
	score_to_board_dst[2].x = 0;
	score_to_board_dst[2].y = 214;

	// High score
	hiscore_to_board_dst[0].x = 222;
	hiscore_to_board_dst[0].y = 234 + 22;
	hiscore_to_board_dst[0].w = 14;
	hiscore_to_board_dst[0].h = 20;

	hiscore_to_board_dst[1].x = 0;
	hiscore_to_board_dst[1].w = 14;
	hiscore_to_board_dst[1].h = 20;
	hiscore_to_board_dst[1].y = 234 + 22;

	hiscore_to_board_dst[2].w = 14;
	hiscore_to_board_dst[2].h = 20;
	hiscore_to_board_dst[2].x = 0;
	hiscore_to_board_dst[2].y = 234 + 22;

	// highscore source rect
	new_hiscore_src.w = 32;
	new_hiscore_src.h = 14;
	new_hiscore_src.x = 516;
	new_hiscore_src.y = 867;

	new_hiscore_dst.w = 32;
	new_hiscore_dst.h = 14;
	new_hiscore_dst.x = 0; // as state var
	new_hiscore_dst.y = 241;

	if (current_score > highest_score)
	{
		new_hiscore_dst.x = 166;
		highest_score = current_score;
		write_highscore();
	}

	Mix_PlayChannel(-1, wooosh_chunk, 0);
}

void game_over_Update(void)
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
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (SDL_PointInRect(&mouse, &play_dst))
				{
					fade_to_black = 1;
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
	bird_update();
	game_over_Draw();
	if (flash){
		alpha -= 15;
		if (alpha == 0) flash = 0;
	}
	if (over_animation){
		if (over_animation_direction == UP){
			over_dst.y -= 1;
			if (over_dst.y == over_dst_top_y) over_animation_direction = DOWN;
		} else if (over_animation_direction == DOWN){
			over_dst.y += 1;
			if (over_dst.y == over_dst_final_y)  over_animation = 0;
		}
	}
	if (fade_to_black){
		fade_to_black_alpha += 5;
		if (fade_to_black_alpha == 255){
			fade_to_black = 0;
			idle_set();
			game_state = GS_IDLE;
		}
	}
}

void game_over_Draw(void)
{
	playing_draw();
	SDL_RenderCopy(Renderer, atlas, &over_src, &over_dst);
	SDL_RenderCopy(Renderer, atlas, &sum_src, &sum_dst);
	if (medal_src.x)
		SDL_RenderCopy(Renderer, atlas, &medal_src, &medal_dst);
	render_score_to_board();
	render_hiscore_to_board();
	if (new_hiscore_dst.x)
		SDL_RenderCopy(Renderer, atlas, &new_hiscore_src, &new_hiscore_dst);
	SDL_RenderCopy(Renderer, atlas, &play_src, &play_dst);
	SDL_RenderCopy(Renderer, atlas, &lb_src, &lb_dst);
	if (fade_to_black)
	{
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, fade_to_black_alpha);
		SDL_RenderFillRect(Renderer, &transition_rect);
	}
}
#pragma endregion game over

void update_ground(void)
{
	grd_src.x += 1;
	if (grd_src.x >= 48 + 584)
		grd_src.x = 584;

	// Check collision with ground
	if (SDL_HasIntersection(&bird_dst, &grd_dst))
	{
		bird_dies();
	}
}

void bird_update(void)
{
	//set bird position
	bird_dst.y += vy / FPS;
	vy += gy / FPS;
	//check upper bound
	if (bird_dst.y < -20)
	{
		vy = 100;
		bird_dst.y = -10;
	}
	//check lower bound
	else if (bird_dst.y > 378)
	{
		vy = 0;
		bird_dst.y = 378;
	}
	//angle
	if (vy > 100)
	{
		bird_angle += 2;
		if (bird_angle > 89)
			bird_angle = 90;
	}
	else if (vy <= -100)
		bird_angle = 340;
	else
		bird_angle = 0;
}

void update_pipes(void)
{
	int i;
	for (i = 0; i < PIPES; i += 2)
	{
		// check if pipe passed player
		if ((pipe_dst[i].x + pipe_dst[i].w) < (bird_dst.x) && (pipe_passed_player[i / 2] == SDL_FALSE))
		{
			pipe_passed_player[i / 2] = SDL_TRUE;
			current_score += 1;
			Mix_PlayChannel(-1, point_chunk, 0);
		}

		// set pipes position
		pipe_dst[i].x -= 1 + PIPE_STEP;
		pipe_dst[i + 1].x -= 1 + PIPE_STEP;

		if (pipe_dst[i].x + pipe_dst[i].w < 0)
		{
			if (i == 0)
			{
				pipe_dst[0].x = pipe_dst[4].x + PIPE_X_OFFSET;
				pipe_dst[0].y = rand() % (PIPE_MAX_Y + 1 - PIPE_MIN_Y) + PIPE_MIN_Y;
				pipe_dst[1].x = pipe_dst[i].x;
				pipe_dst[1].y = pipe_dst[i].y - (pipe_src.h + PIPE_OPEN);
				pipe_passed_player[0] = SDL_FALSE;
			}
			if (i == 2)
			{
				pipe_dst[2].x = pipe_dst[0].x + PIPE_X_OFFSET;
				pipe_dst[2].y = rand() % (PIPE_MAX_Y + 1 - PIPE_MIN_Y) + PIPE_MIN_Y;
				pipe_dst[3].x = pipe_dst[i].x;
				pipe_dst[3].y = pipe_dst[i].y - (pipe_src.h + PIPE_OPEN);
				pipe_passed_player[1] = SDL_FALSE;
			}
			if (i == 4)
			{
				pipe_dst[4].x = pipe_dst[2].x + PIPE_X_OFFSET;
				pipe_dst[4].y = rand() % (PIPE_MAX_Y + 1 - PIPE_MIN_Y) + PIPE_MIN_Y;
				pipe_dst[5].x = pipe_dst[i].x;
				pipe_dst[5].y = pipe_dst[i].y - (pipe_src.h + PIPE_OPEN);
				pipe_passed_player[2] = SDL_FALSE;
			}
		}
	}

	// Check collision with pipes
	for (i = 0; i < PIPES; i++)
	{
		if (SDL_HasIntersection(&bird_dst, &pipe_dst[i]))
		{
			bird_dies();
		}
	}
}

void bird_dies(void){
	Mix_PlayChannel(-1, hit_chunk, 0);
	Mix_PlayChannel(-1, collide_chunk, 0);
	alpha = 255;
	flash = 1;
	game_over_set();
	game_state = GS_OVER;
}

void bird_flap(void)
{
	// set bird anim state var
	flap_counter += flap_delay;
	if (flap_counter > 167)
	{
		flap_state = 1;
		flap_counter = 0;
	}
	//select bird frame
	if (flap_state)
	{
		if (bird_src.x == 355)
			bird_src.x = 390;
		else if (bird_src.x == 390)
			bird_src.x = 425;
		else if (bird_src.x == 425)
			bird_src.x = 355;
		flap_state = 0;
	}
}

void bird_slide(void)
{
	if (bird_flip_direction)
	{
		bird_dst.y++;
		if (bird_dst.y >= 250)
			bird_flip_direction = 0;
	}
	else
	{
		bird_dst.y--;
		if (bird_dst.y <= 200)
			bird_flip_direction = 1;
	}
}

#pragma region score
void render_score(void)
{

	if (current_score > 99)
	{
		// space between digits = 4 Pixel
		number_dst[0].x = (ww / 2 - number_dst[0].w / 2) - 2 * (number_dst[1].w - 4);
		number_dst[1].x = number_dst[0].x + number_dst[1].w + 4;
		number_dst[2].x = number_dst[1].x + number_dst[1].w + 4;

		SDL_RenderCopy(Renderer, atlas, &number_src[((current_score - (current_score % 10)) / 100) % 10], &number_dst[0]);
		SDL_RenderCopy(Renderer, atlas, &number_src[((current_score - (current_score % 10)) / 10) % 10], &number_dst[1]);
		SDL_RenderCopy(Renderer, atlas, &number_src[current_score % 10], &number_dst[2]);
	}
	else if (current_score > 9)
	{
		// space between digits = 4 Pixel
		number_dst[0].x = (ww / 2 - number_dst[0].w / 2) - (number_dst[1].w - 4);
		number_dst[1].x = number_dst[0].x + number_dst[1].w + 4;
		SDL_RenderCopy(Renderer, atlas, &number_src[(current_score - (current_score % 10)) / 10], &number_dst[0]);
		SDL_RenderCopy(Renderer, atlas, &number_src[current_score % 10], &number_dst[1]);
	}
	else
		SDL_RenderCopy(Renderer, atlas, &number_src[current_score], &number_dst[0]);
}

void render_score_to_board(void)
{

	if (current_score > 99)
	{
		score_to_board_dst[0].x = 222 - 15 * 2;
		score_to_board_dst[1].x = 222 - 15;
		score_to_board_dst[2].x = 222;
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[((current_score - (current_score % 10)) / 100) % 10], &score_to_board_dst[0]);
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[((current_score - (current_score % 10)) / 10) % 10], &score_to_board_dst[1]);
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[current_score % 10], &score_to_board_dst[2]);
	}
	else if (current_score > 9)
	{
		score_to_board_dst[0].x = 222 - 15;
		score_to_board_dst[1].x = 222;
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[(current_score - (current_score % 10)) / 10], &score_to_board_dst[0]);
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[current_score % 10], &score_to_board_dst[1]);
	}
	else
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[current_score], &score_to_board_dst[0]);
}

void render_hiscore_to_board(void)
{

	if (highest_score > 99)
	{
		hiscore_to_board_dst[0].x = 222 - 15 * 2;
		hiscore_to_board_dst[1].x = 222 - 15;
		hiscore_to_board_dst[2].x = 222;
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[((highest_score - (highest_score % 10)) / 100) % 10], &hiscore_to_board_dst[0]);
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[((highest_score - (highest_score % 10)) / 10) % 10], &hiscore_to_board_dst[1]);
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[highest_score % 10], &hiscore_to_board_dst[2]);
	}
	else if (highest_score > 9)
	{
		hiscore_to_board_dst[0].x = 222 - 15;
		hiscore_to_board_dst[1].x = 222;
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[(highest_score - (highest_score % 10)) / 10], &hiscore_to_board_dst[0]);
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[highest_score % 10], &hiscore_to_board_dst[1]);
	}
	else
		SDL_RenderCopy(Renderer, atlas, &score_to_board_src[highest_score], &hiscore_to_board_dst[0]);
}

void read_highscore(void)
{
	SDL_RWops *file = NULL;
	file = SDL_RWFromFile("hiscore", "r");
	if (file)
	{
		SDL_RWread(file, &highest_score, sizeof(int), 1);
		SDL_RWclose(file);
	}
	else
	{
		highest_score = 0;
		write_highscore();
	}
}

void write_highscore(void)
{
	SDL_RWops *file = SDL_RWFromFile("hiscore", "w+");
	SDL_RWwrite(file, &highest_score, sizeof(int), 1);
	SDL_RWclose(file);
}
#pragma endregion score

void assets_out(void)
{
	Mix_FreeChunk(collide_chunk);
	Mix_FreeChunk(flap_chunk);
	Mix_FreeChunk(hit_chunk);
	Mix_FreeChunk(point_chunk);
	Mix_FreeChunk(wooosh_chunk);

	SDL_DestroyTexture(atlas);
}
#pragma endregion Functions