CC=g++
FLAGS=-lwiringPi -lgps
MAIN=final.cpp
SOURCES=udp/udp.cpp spi/spi.cpp hmc5883l/hmc5883l.h helper/helper.cpp autobot/autobot.cpp
EXECUTABLE=final
RM=rm -rf

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(FLAGS) $(MAIN) $(SOURCES) -o $@

control:
	$(CC) $(FLAGS) control.cpp udp/udp.cpp spi/spi.cpp -o control

clean:
	$(RM) *.o $(EXECUTABLE) control
