#include "GPstation.h"


void overlay_vram(UINT8 *cpu_memory, UINT8 *gpu_memory)
{
	int x,y;
	for(y = 0; y < 480; y++) {
		for(x = 0; x < 640; x++) {
			UINT32 col = gpu_memory[y * 640 + x];
			if(col != 0x000000) ((UINT32 *)cpu_memory + 0x10000)[y * 640 + x] = col;
		}
	}
}

void reflesh_vram(UINT8 *cpu_memory, SDL_Surface *sdl_screen)
{
	memcpy(sdl_screen->pixels, cpu_memory + 0x10000, 640*480*4);
}