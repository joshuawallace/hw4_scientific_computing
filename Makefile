

CXXFLAGS = -g -Wall
CFLAGS = -g -Wall
CFLAGSLM = -g -Wall -lm


all: heat_serial


stepper: stepper.o boundaryconditions.o creategrid.o
	$(CC) -c -o $@ $^ $(CFLAGS)

serial_objects = heat_serial.o creategrid.o stepper.o
heat_serial: $(serial_objects)
	$(CC) -o $@ $^ $(CFLAGSLM)





clean:
	$(RM) *.o
	$(RM) .depen
