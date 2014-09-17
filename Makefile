
CC=g++
CFLAGS=-Wall -g # -std=c++11
LFLAGS=-g
LIBS=-lboost_regex -lSDL -lSDL_image

EXE=adventure
SOURCES= \
	main.cc \
	engine.cc \
	room.cc \
	interface.cc \
	player.cc \
	world.cc \
	loader.cc \
	item.cc \
	description.cc \
	exit.cc \
	sdl/screen.cc \
	sdl/event.cc \
	sdl/surface.cc

OBJECTS= $(patsubst %.cc,%.o,$(SOURCES)) 
HEADERS= $(patsubst %.cc,%.hh,$(SOURCES)) 

all: $(OBJECTS)
	$(CC) -o $(EXE) $(LFLAGS) $(OBJECTS) $(LIBS)

.cc.o:
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(EXE)
	rm -f *.o

tags: FORCE
	ctags *.cc *.hh

FORCE:
