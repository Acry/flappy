#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include "helper_1.h"
SDL_Window *Window = NULL;
SDL_Renderer *Renderer = NULL;

void init(void)
{

  SDL_Init(SDL_INIT_EVERYTHING);
  Window = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN);

#pragma region ICON
  SDL_Surface *icon;
  icon = IMG_Load("../../assets/gfx/icon.png");
  SDL_SetWindowIcon(Window, icon);
  SDL_FreeSurface(icon);
#pragma endregion ICON

  Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawBlendMode(Renderer, SDL_BLENDMODE_BLEND);
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  Mix_Init(MIX_INIT_OGG);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void exit_(void)
{
  Mix_CloseAudio();
  Mix_Quit();
  SDL_DestroyRenderer(Renderer);
  SDL_DestroyWindow(Window);
  SDL_Quit();
}
