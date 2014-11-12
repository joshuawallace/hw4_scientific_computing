

CXXFLAGS = -g -Wall
CFLAGS = -g -Wall


all: heat_serial


serial_objects = heat_serial.o
heat_serial: $(serial_objects)
	$(CC) -o $@ $^ $(CFLAGS)





clean:
	$(RM) *.o
	$(RM) .depen
