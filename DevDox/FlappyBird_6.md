# Flappy Bird - C/SDL2-Implementation - Part 6

<< [Part 5](FlappyBird_5.md) | [TOC](TOC.md) | [Part 7](Patience.md) >><br>

## Effects:

### Screen transitions

#### White death flash

I am gonna let the screen flash white when the bird dies.

When does the bird die?
Either when it hits the ground or when it collides with a pipe.


Function: `playing_update`
```c
// Check collision with ground
HasIntersection(&bird_dst, &grd_dst)
```

```c
if (SDL_HasIntersection(&bird_dst, &grd_dst))
{
    Mix_PlayChannel(-1, hit_chunk, 0);
    Mix_PlayChannel(-1, collide_chunk, 0);
    game_over_set();
    game_state = GS_OVER;
}
```

Function: `update_pipes`

```c
// Check collision with pipes
HasIntersection(&bird_dst, &pipe_dst[i])
```

```c
for (i = 0; i < PIPES; i++)
{
	if (SDL_HasIntersection(&bird_dst, &pipe_dst[i]))
	{
		Mix_PlayChannel(-1, hit_chunk, 0);
		Mix_PlayChannel(-1, collide_chunk, 0);
		game_over_set();
		game_state = GS_OVER;

	}
}
```

The code that is executed when the bird dies is the same in both cases.
So I will write a function that is called when the bird dies.

Let me refactor it.

The containing code is:

```c
Mix_PlayChannel(-1, hit_chunk, 0);
Mix_PlayChannel(-1, collide_chunk, 0);
game_over_set();
game_state = GS_OVER;
```
This will be the function body.


Gets no param, doesn't return anything. I will call it `bird_dies`.

This is the final function:

Prototype:
```c
void bird_dies(void);
```

Implementation:
```c
void bird_dies(void){
	Mix_PlayChannel(-1, hit_chunk, 0);
	Mix_PlayChannel(-1, collide_chunk, 0);
	game_over_set();
	game_state = GS_OVER;
}
```

It compiles, it runs - the behavior is the same.

One more thing:  
`bird_dies` is called in `playing_update` and  `update_pipes`.
For more consistence I put the collision checking for ground into `update_ground` and rename the function to `update_bird` to `bird_update`.

And in `playing_update` I change the order of function calls to:

```c
update_pipes();
update_ground();
bird_update();
bird_flap();
playing_draw();
```

Alright, now there is one place to emit a flash. And the flash will only be visible during `playing_draw`.

There will be a white rect on top of the render stack, that reduces its alpha over time.

transition_rect will be that white rect on top .
`SDL_Rect transition_rect;`

We need to initialize the rect.
I will do that in `assets_in`.

The state var to track if the `transition_rect` should be rendered:  
```c
char flash;
```

The code snippet to actually render the rect when `flash` is set:

```c
if (flash)
{
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, alpha);
	SDL_RenderFillRect(Renderer, &transition_rect);
}
```

The variable that holds the alpha value:
```c
Uint8 alpha;
```


in `playing_set` I set alpha to 0:

```c
alpha = 0;
```

When the bird dies stuff gets triggered:  

`bird_dies`

```c
alpha = 255;
flash = 1;
```

Now the playing screen renders a white overlay, as long as `flash` is set.
To reduce the `alpha` and unset `flash` once `alpha` is 0, I need some kind of logic and a place to insert it to alter the `alpha` value.

`game_over_update`
```c
if (flash){
	alpha -= 15;
	if (alpha == 0) flash = 0;
}
```

## Retrospective

## Outlook

<< [Part 5](FlappyBird_5.md) | [TOC](TOC.md) | [Part 7](Patience.md) >><br>
