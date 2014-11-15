CXXFLAGS = -g -Wall -std=c99
CFLAGS = -g -Wall -lm -std=c99
OMPFLAGS = -fopenmp



all: heat_serial #heat_serial.e


serial_objects = heat_serial.o creategrid.o stepper.o boundaryconditions.o
heat_serial: $(serial_objects)
	$(CC) -o $@ $^ $(CFLAGS)

#heat_serial.e: heat_serial.e.o creategrid.o stepper.e.o boundaryconditions.o
#	$(CC) -o $@ $^ $(CFLAGS)
omp_objects = heat_omp.o creategrid.o stepper.omp.o boundaryconditions.o
heat_omp: $(omp_objects)
	$(CC) -o $@ $^ $(CFLAGS) $(OMPFLAGS)

clean:
	$(RM) *.o
	$(RM) .depend

depend:
	$(CXX) -MM $(CXXFLAGS) *.cc > .depend

-include .depend






