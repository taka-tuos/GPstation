TARGET		= GPstation
OBJS_TARGET	= GPstation.o graphic.o sdl.o io.o gp3200+/gp3200+.o gp3200+/float32.o

CFLAGS += -O4 `sdl-config --cflags`
LIBS += `sdl-config --libs`

include Makefile.in
