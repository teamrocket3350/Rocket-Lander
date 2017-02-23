CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: rocketlander

<<<<<<< HEAD
rocketlander: rocketlander.cpp log.cpp mytime.cpp
	g++ $(CFLAGS) rocketlander.cpp log.cpp mytime.cpp libggfonts.a ramonR.cpp -Wall -Wextra $(LFLAGS) -o rocketlander
=======
rocketlander: rocketlander.cpp log.cpp mytime.cpp ramonR.cpp abrahamA.cpp patrickE.cpp nicholasP.cpp
	g++ $(CFLAGS) rocketlander.cpp log.cpp mytime.cpp nicholasP.cpp ramonR.cpp abrahamA.cpp patrickE.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rocketlander
>>>>>>> 3427ef19ea2afcc451502ea6d95621735cb2dc22

clean:
	rm -f rocketlander
	rm -f *.o
