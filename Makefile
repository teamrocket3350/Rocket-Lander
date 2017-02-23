CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: rocketlander

<<<<<<< HEAD
rocketlander: rocketlander.cpp log.cpp mytime.cpp abrahamA.cpp
	g++ $(CFLAGS) rocketlander.cpp abrahamA.cpp log.cpp mytime.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rocketlander
=======
rocketlander: rocketlander.cpp log.cpp mytime.cpp RamonR.cpp abrahamA.cpp PatrickE.cpp nicholasP.cpp
	g++ $(CFLAGS) rocketlander.cpp log.cpp mytime.cpp nicholasP.cpp RamonR.cpp abrahamA.cpp PatrickE.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rocketlander
>>>>>>> 8c07361e9bf5626cdfb22cfbf2dc329779a336e4

clean:
	rm -f rocketlander
	rm -f *.o
