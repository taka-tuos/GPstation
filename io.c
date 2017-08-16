#include "GPstation.h"

extern int req_reflesh;

SDL_TimerID timer;

Uint32 timer_stop(Uint32 interval, void *cpu)
{
	SDL_RemoveTimer(timer);
	((gp3200_cpu *)cpu)->r_pc = *((UINT32 *)&(((gp3200_cpu *)cpu)->r_memory[0]));
	((gp3200_cpu *)cpu)->r_segs[0] = *((UINT32 *)&(((gp3200_cpu *)cpu)->r_memory[4]));
	return 0;
}

void write_io(void *dat, int address, int data)
{
	gp3200_cpu *cpu = (gp3200_cpu *)dat;

	switch(address) {
	case 0x00:
		timer = SDL_AddTimer(data,timer_stop,cpu);
		break;
	case 0x01:
		req_reflesh = 1;
		break;
	}
}

int read_io(void *dat, int address)
{
	gp3200_cpu *cpu = (gp3200_cpu *)dat;

	return 0;
}

void msec_wait(int msec)
{
	SDL_Delay(msec);
}
