#include "GPstation.h"

Uint8 button;

int poll_event(SDL_Event *sdl_event)
{
	Uint8 *key_state = SDL_GetKeyState(NULL);

	if(SDL_PollEvent(sdl_event)) {
		switch (sdl_event->type) {
		case SDL_KEYDOWN:
			button = sdl_event->key.keysym.sym;
			break;
		case SDL_KEYUP:
			button = 0;
			break;
		case SDL_QUIT:
			return 1;
		}
	}

	return 0;
}