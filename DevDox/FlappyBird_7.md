# Flappy Bird - C/SDL2-Implementation - Part 7

<< [Part 6](FlappyBird_6.md) | [TOC](TOC.md) | [Part 8](Patience.md) >><br>

## Animations

Linear Animations: Images are shown at another spot at a later point in time.

### Game over text moves


![](Images/game-over.png)

How does the Animation look like?

Actually the game-over text fades in with zero opacity to full opacity, but I won't implement that.

From observations of the video one can tell that it moves first up an then down.

Always good to slow down playback speed or watch it frame by frame.

The starting point in time is somewhere after the bird died.

Basically this needs to happen

```text
set animation to start
    over_animation = 1
set direction = up
set start y - somewhat over end y
set top y
when reaches top y, set direction to down

set end y
set animation finish
over_animation = 1
```

Do not forget SDL's inverted y.
0 is top 0+n down.

Where is it rendered?

game over draw
`game_over_Draw`

`SDL_RenderCopy(Renderer, atlas, &over_src, &over_dst);`

To make the code easier to read, I use a new enum.

```c
enum direction
{
	UP,
	DOWN
};
```

```c
enum direction over_animation_direction;
```

```c
int over_dst_final_y;
int over_dst_top_y;
char over_animation;
enum direction over_animation_direction;
```

game over set `game_over_set`

```c
over_dst_top_y = 120;
over_dst_final_y = 130;
over_animation = 1;
over_animation_direction = UP;
```

game over update `game_over_Update`   

Need to alter the `over_dst` rect
something like (pseudo code):  

```text
if over_animation
    if game_over_animation_direction == up
        over_dst.y+= some_factor
        if over_dst.y == top_endpoint game_over_animation_direction = down
    else
        over_dst.y-= some_factor
        if over_dst.y == bottom_endpoint over_animation = 0
```

```c
if (over_animation){
    if (over_animation_direction == UP){
        over_dst.y -= 1;
        if (over_dst.y == over_dst_top_y) over_animation_direction = DOWN;
    } else if (over_animation_direction == DOWN){
        over_dst.y += 1;
        if (over_dst.y == over_dst_final_y)  over_animation = 0;
    }
}
```

That's it. The image is moving.
### Scoreboard slides in

The Scoreboard slides in from the bottom.

Altered is: `sum_dst.y`

`game_over_set`
```c
sum_animation = 1;
sum_dst_final_y = 184;
sum_dst_start_y = wh; // 512
```

`game_over_Update`

The `else if` waits for the game over animation.
```c
 else if (sum_animation){
		sum_dst.y -= 12;
		if (sum_dst.y <= sum_dst_final_y){
			sum_dst.y =  sum_dst_final_y;
			sum_animation = 0;
		}
	}
```

`game_over_Draw`  
Now I wait for the animation, before I render the rest
```c
if (!sum_animation){
    if (medal_src.x)
        SDL_RenderCopy(Renderer, atlas, &medal_src, &medal_dst);
    if (new_hiscore_dst.x)
        SDL_RenderCopy(Renderer, atlas, &new_hiscore_src, &new_hiscore_dst);
    render_score_to_board();
    render_hiscore_to_board();
    SDL_RenderCopy(Renderer, atlas, &play_src, &play_dst);
    SDL_RenderCopy(Renderer, atlas, &lb_src, &lb_dst);
}
```

### The score counts up

### Sparkling medals

## Retrospective

## Outlook

<< [Part 6](FlappyBird_6.md) | [TOC](TOC.md) | [Part 8](Patience.md) >><br>
