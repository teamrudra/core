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

test:
	$(CC) $(FLAGS) test.cpp udp/udp.cpp helper/helper.cpp -o test

clean:
	$(RM) *.o $(EXECUTABLE) control
