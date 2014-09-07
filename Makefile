
CC=g++
CFLAGS=-Wall -g # -std=c++11
LFLAGS=-g -std=c++11 -lboost_regex

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
	$(CC) -o $(EXE) $(LFLAGS) $(OBJECTS)

.cc.o:
	$(CC) -c $< $(CFLAGS)

clean:
	[ -e $(EXE) ] && rm $(EXE) || true
	rm *.o

tags:
	ctags *.cc *.hh


