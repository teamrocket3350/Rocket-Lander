CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: rocketlander rocketlander-debug

rocketlander: rocketlander.cpp log.cpp mytime.cpp ppm.cpp \
    ramonR.cpp ramonR.h \
    abrahamA.cpp abrahamA.h \
    patrickE.cpp patrickE.h \
    nicholasP.cpp nicholasP.h
	
	g++ $(CFLAGS) rocketlander.cpp log.cpp mytime.cpp ppm.cpp\
	    nicholasP.cpp \
	    ramonR.cpp \
	    abrahamA.cpp \
	    patrickE.cpp \
	    libggfonts.a \
	    /usr/lib/x86_64-linux-gnu/libopenal.so \
	    /usr/lib/x86_64-linux-gnu/libalut.so \
	    -Wall -Wextra $(LFLAGS) -o rocketlander

# Temporary debug build so Abraham can implement his level loading
rocketlander-debug: rocketlander.cpp log.cpp mytime.cpp ppm.cpp \
    ramonR.cpp ramonR.h \
    abrahamA.cpp abrahamA.h \
    patrickE.cpp patrickE.h \
    nicholasP.cpp nicholasP.h
	
	g++ $(CFLAGS) rocketlander.cpp log.cpp mytime.cpp ppm.cpp\
	    nicholasP.cpp \
	    ramonR.cpp \
	    abrahamA.cpp \
	    patrickE.cpp \
	    libggfonts.a \
	    /usr/lib/x86_64-linux-gnu/libopenal.so \
	    /usr/lib/x86_64-linux-gnu/libalut.so \
	    -Wall -Wextra $(LFLAGS) -Ddebug -o rocketlander-ab


clean:
	rm -f rocketlander
	rm -f rocketlander-ab
	rm -f *.o
