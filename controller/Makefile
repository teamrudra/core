CC=g++
FLAGS=-lwiringPi
SOURCES=main.cpp udp/udp.cpp spi/spi.cpp hmc5883l/hmc5883l.h
EXECUTABLE=rudra
RM=rm -rf

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(FLAGS) $(SOURCES) -o $@

clean:
	$(RM) *.o $(EXECUTABLE)
