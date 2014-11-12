

CXXFLAGS = -g -Wall


all: heat_serial


serial_objects = heat_serial.o
heat_serial: $(serial_objects)
	$(CXX) -o $@ $^





clean:
	$(RM) *.o
	$(RM) .depen
