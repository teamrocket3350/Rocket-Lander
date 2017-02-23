CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: rocketlander

rocketlander: rocketlander.cpp log.cpp mytime.cpp
	g++ $(CFLAGS) rocketlander.cpp log.cpp mytime.cpp PatrickE.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rocketlander

clean:
	rm -f rocketlander
	rm -f *.o
