
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
	input_handler.cc \
	player.cc \
	world.cc \
	loader.cc \
	item.cc

OBJECTS= $(patsubst %.cc,%.o,$(SOURCES)) 
HEADERS= $(patsubst %.cc,%.hh,$(SOURCES)) 

all: $(OBJECTS)
	$(CC) -o $(EXE) $(LFLAGS) $(OBJECTS) $(LIBS)

.cc.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(EXE)
	rm -f *.o

tags:
	ctags *.cc *.hh


