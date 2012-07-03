CC=g++
CFLAGS=-c -Wall
LDFLAGS=
INC=-Isrc/framework -Isrc/model -Isrc/view - Isrc/controller -I/usr/include/libxml2 -I/usr/include/allegro5
LIB=-lallegro -lallegro_main -lallegro_image -lallegro_primitives -lxml2
SOURCES=$(wildcard src/**/*.cpp) src/main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=LPC2012Game

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIB) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@


