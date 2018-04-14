CC=g++
FLAGS=-lwiringPi -lgps
SOURCES=final.cpp udp/udp.cpp spi/spi.cpp hmc5883l/hmc5883l.h helper/helper.cpp autobot/autobot.cpp
EXECUTABLE=final
RM=rm -rf

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(FLAGS) $(SOURCES) -o $@

clean:
	$(RM) *.o $(EXECUTABLE)
