CC=g++
FLAGS=-lwiringPi -lgps
SOURCES=compass.cpp
EXECUTABLE=compass
RM=rm -rf

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $(SOURCES) -o $@

clean:
	$(RM) *.o $(EXECUTABLE)

