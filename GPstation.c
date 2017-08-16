#include "GPstation.h"
#include <time.h>

#define CPU_IPS			32
#define GPU_IPS			4

int req_reflesh = 0;

void load_prg(gp3200_cpu *core, int baseaddr, char *binname, int maxsize);

/*
	-- Memory Map (CPU) --
	0x00000000-0x0000ffff : IVT(64KB)
	0x00010000-0x0030ffff : VRAM(32bpp,3MB)
	0x00310000-0x0130ffff : GIOS(16MB)
	0x01310000-0x0330ffff : GAME PRG(32MB)
	0x03310000-0x0331ffff : Stack
	0x03320000-0x04000000 : Free Area
	Total : 64MB
	
	-- Memory Map (GPU) --
	0x00000000-0x002fffff : VRAM(CPU Memory link)
	0x00300000-0x012fffff : GIOS for GPU(16MB)
	0x01300000-0x0130ffff : Stack
	0x01310000-0x02000000 : Free Area
	Total : 32MB
*/

int cpu_clock = 0;

int gp3200_thread(void *core_gp3200_cpu)
{
	while (1) {
		gp3200_step((gp3200_cpu*)core_gp3200_cpu);
		cpu_clock++;
	}
	return 0;
}

int main(int argc, char **argv) {
	SDL_Event sdl_event;
	gp3200_cpu core_gp3200_cpu;
	gp3200_cpu core_gp3200_gpu;
	UINT8 *cpu_memory;
	UINT8 *gpu_memory;
	int cpu_ips;
	int gpu_ips;
	SDL_Surface *sdl_screen;

	//int cpu_clock = 0;
	int gpu_clock = 0;

	/*if(argc < 2) {
		puts("using>GPstation <romfile>\n");
		exit(-1);
	}*/

	cpu_memory = (UINT8 *)malloc(64*1024*1024);
	gpu_memory = (UINT8 *)malloc(32*1024*1024);

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)) exit(1);

	sdl_screen = SDL_SetVideoMode(640,480,32,SDL_HWSURFACE);
	SDL_WM_SetCaption("GPstation", NULL);

	gp3200_init(&core_gp3200_cpu, cpu_memory);
	gp3200_init(&core_gp3200_gpu, gpu_memory);

	core_gp3200_cpu.r_pc = 0x00310000;
	core_gp3200_gpu.r_pc = 0x00300000;
	core_gp3200_cpu.r_regs[32] = 0x03320000;
	core_gp3200_gpu.r_regs[32] = 0x01310000;
	core_gp3200_cpu.r_segs[0] = 0;
	core_gp3200_gpu.r_segs[0] = 0;

	core_gp3200_cpu.on_ior = read_io;
	core_gp3200_cpu.on_iow = write_io;
	core_gp3200_cpu.on_wait = msec_wait;

	core_gp3200_gpu.r_memory[0x00300000] = 0xff;

	memset(core_gp3200_cpu.r_memory + 0x10000, 0, 640*480*4);
	memset(core_gp3200_gpu.r_memory, 0, 640*480*4);

	int i;
	for(i = 0; i < 32; i++) core_gp3200_cpu.r_regs[i] = 0;
	for(i = 0; i < 32; i++) core_gp3200_gpu.r_regs[i] = 0;

	load_prg(&core_gp3200_cpu,0x00310000,"gios.bin",16*1024*1024);

	int fsc = SDL_GetTicks();
	int fcc = SDL_GetTicks();

	SDL_CreateThread(gp3200_thread, &core_gp3200_cpu);

	while(!poll_event(&sdl_event)) {
		//for(cpu_ips = SDL_GetTicks(); SDL_GetTicks() - cpu_ips < CPU_IPS; cpu_clock++) gp3200_step(&core_gp3200_cpu);
		//for(gpu_ips = SDL_GetTicks(); SDL_GetTicks() - gpu_ips < GPU_IPS; gpu_clock++) gp3200_step(&core_gp3200_gpu);

		if(req_reflesh) {
			overlay_vram(cpu_memory, gpu_memory);
			reflesh_vram(cpu_memory, sdl_screen);
			SDL_UpdateRect(sdl_screen,0,0,0,0);
			req_reflesh = 0;
		}

		if(SDL_GetTicks() - fsc >= 1000) {
			char s[256];
			sprintf(s,"GPstation   CPU %.1fMCycle GPU %.1fMCycle",(float)cpu_clock/1000.0f/1000.0f,(float)gpu_clock/1000.0f/1000.0f);
			SDL_WM_SetCaption(s, NULL);
			cpu_clock = gpu_clock = 0;
			fsc = SDL_GetTicks();
		}
	}
}

void load_prg(gp3200_cpu *core, int baseaddr, char *binname, int maxsize)
{
	FILE *fp = fopen(binname,"rb");
	if(fp == NULL) return;

	fread(core->r_memory + baseaddr,sizeof(char),maxsize,fp);

	fclose(fp);
}
