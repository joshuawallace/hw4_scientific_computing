CXXFLAGS = -g -Wall -std=c99
CFLAGS = -g -Wall -std=c99
CFLAGSLM = -g -Wall -lm -std=c99


all: heat_serial

#boundaryconditions: boundaryconditions.o
#	$(CC) -c -o $@ $^ $(CFLAGSLM)

#stepper: stepper.o boundaryconditions.o creategrid.o
#	$(CC) -c -o $@ $^ $(CFLAGS)

serial_objects = heat_serial.o creategrid.o stepper.o boundaryconditions.o
heat_serial: $(serial_objects)
	$(CC) -o $@ $^ $(CFLAGSLM)

heat_serial.e: heat_serial.e.o creategrid.o stepper.e.o boundaryconditions.o
	$(CC) -o $@ $^ $(CFLAGSLM)

clean:
	$(RM) *.o
	$(RM) .depend

depend:
	$(CXX) -MM $(CXXFLAGS) *.cc > .depend

-include .depend






