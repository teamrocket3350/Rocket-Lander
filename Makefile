CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: rocketlander

rocketlander: rocketlander.cpp log.cpp mytime.cpp ramonR.cpp ramonR.h abrahamA.cpp abrahamA.h patrickE.cpp patrickE.h nicholasP.cpp nicholasP.h
	g++ $(CFLAGS) rocketlander.cpp log.cpp mytime.cpp nicholasP.cpp ramonR.cpp abrahamA.cpp patrickE.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rocketlander

clean:
	rm -f rocketlander
	rm -f *.o
