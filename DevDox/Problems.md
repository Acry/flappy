# Issues

### Trouble

Encountering bugs during development lies in the nature of programming.
Here are a few problems I faced during the implementation.

#### BUGS

CRITICAL BUG - CORE DUMPED
Switching state to game over
frequent but no constant

Solution:

```c
void read_highscore(void)
{
highscore_counter=0;
```

#### Major Bug

Score was not counted correctly
It is a timing issue.
Game loop is still not implemented good.

Major Bug
After render scaling the mouse coordinates are screwed.
SDL_RenderSetLogicalSize
SDL_RenderSetViewport
SDL_RenderSetViewport(SDL_Renderer* renderer,
const SDL_Rect* rect)

Minor BUG
flap only on key press, no repeat

Minor BUG
Bird disappears after dying
