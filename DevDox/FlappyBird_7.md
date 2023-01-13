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

Do not forget SDL's inverted y-axis.
0 is top, 0+n goes down.

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

Goal:  
`render_score_to_board` renders each score digit pretty quickly counting up.

Introduce a new var: counting_score  

In `game_over_set` I set `counting_score` to `0`  

In `render_score_to_board` substitute `current_score` with `counting_score`.

And increase `counting_score` while smaller than `current_score` in `game_over_update`

Updating a number goes to ...update
```c
if (!sum_animation){
    if (counting_score < current_score)
        counting_score++;
}
```

Actually I could leave it as it is, but I am not satisfied right now, because
there is one problem left, the animation speed. Since the state integration is still a feature problem I will discuss in depth, I need a simple solution for now, cutting the animation speed to the half would be enough for the moment.

How can I do that?

The current heartbeat of the game is v-sync.

Since need a counter for the next section too, I will create a counter which raises every 16.67 ms.

```c
int anim_counter;
```

`game_over_set`
```c
anim_counter = 0;
```

`game_over_update`
```c
anim_counter++;
```
If I want to trigger every ~200 ms (200/17=~12) I need to check 
if `anim_counter` is divisible `12`.

Gonna use modulo here:
`12%12=0`, `24%12=0`
```c
!(anim_counter%12)
```

```c
if (!sum_animation && !(anim_counter%12)){
    if (counting_score < current_score)
        counting_score++;
}
```

Now the counter is way slower than in the original game, but for demonstration purposes and small scores it is good enough.

Wonderful, head on to the last animation.

### Sparkling medals

![](./Images/sparkles.png)
![](./Images/gold_medal.png)

Problems to solve:  
switch src rect randomly between 1-3  
show dst rect randomly
show it on the circle of a medal

rand  
circle not square  
https://stackoverflow.com/a/50746409/9721479

https://rosettacode.org/wiki/Constrained_random_points_on_a_circle#C

```text
SDL_Rect spark_src;
SDL_Rect spark_dst;

float spark_counter = 0;
float sparkle_anim_counter;## Retrospective

char sparkle_anim;
char spark = 0;

//sparkles
spark_src.x = 245;
spark_src.y = 717;
spark_src.w = 10;
spark_src.h = 10;

spark_dst.x = 0;
spark_dst.y = 0;
spark_dst.w = 10;
spark_dst.h = 10;
    
spark_dst.x = rand() % ((medal_dst.x + medal_dst.w) + 1 - medal_dst.x) + medal_dst.x;

spark_dst.y = rand() % ((medal_dst.y + medal_dst.h) + 1 - medal_dst.y) + medal_dst.y;## Outlook

//change sparkle frame
if (sparkle_anim)
{
    if (spark_src.y == 717)
    {
        spark_src.y = 727;
    }
    else if (spark_src.y == 727)
    {
        spark_src.y = 741;
    }
    else if (spark_src.y == 741)
    {
        spark_src.y = 717;
    }
    sparkle_anim = 0;
}
//END 	SPARKLES

SDL_RenderCopy(Renderer, Texture, &spark_src, &spark_dst);
```

<< [Part 6](FlappyBird_6.md) | [TOC](TOC.md) | [Part 8](Patience.md) >><br>
