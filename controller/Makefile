CC=g++
FLAGS=-lwiringPi -lgps
SOURCES=main.cpp udp/udp.cpp spi/spi.cpp hmc5883l/hmc5883l.h Rudra/rudra.cpp
EXECUTABLE=rudra
RM=rm -rf

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(FLAGS) $(SOURCES) -o $@

clean:
	$(RM) *.o $(EXECUTABLE)
