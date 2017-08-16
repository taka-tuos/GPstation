#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include "gp3200+/gp3200+.h"

int poll_event(SDL_Event *sdl_event);

void overlay_vram(UINT8 *cpu_memory, UINT8 *gpu_memory);
void reflesh_vram(UINT8 *cpu_memory, SDL_Surface *sdl_screen);
void msec_wait(int msec);

void write_io(void *dat, int address, int data);
int read_io(void *dat, int address);
