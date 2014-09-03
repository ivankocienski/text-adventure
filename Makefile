
CC=g++
CFLAGS=-Wall -g
LFLAGS=-g

EXE=adventure
SOURCES= \
	main.cc \
	engine.cc \
	room.cc \
	interface.cc \
	input_handler.cc \
	player.cc \
	world.cc

OBJECTS= $(patsubst %.cc,%.o,$(SOURCES)) 

all: $(OBJECTS)
	$(CC) -o $(EXE) $(LFLAGS) $(OBJECTS)

.cc.o:
	$(CC) -c $< $(CFLAGS)

#engine.o: engine.cc
#	$(CC) -c engine.cc $(CFLAGS)
#
#main.o: main.cc
#	$(CC) -c main.cc $(CFLAGS)
#
#room.o: room.cc
#	$(CC) -c room.cc $(CFLAGS)
#
#interface.o: interface.cc
#	$(CC) -c interface.cc $(CFLAGS)
#
#input_handler.o: input_handler.cc
#	$(CC) -c input_handler.cc $(CFLAGS)

clean:
	[ -e $(EXE) ] && rm $(EXE)
	for obj in "$(OBJECTS)"; do [ -e $obj ] && rm $obj; done



