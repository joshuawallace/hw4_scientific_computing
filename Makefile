CXXFLAGS = -g -Wall -std=c99
CFLAGS = -g -Wall -lm -std=c99
CFLAGSMPI = -g -Wall -std=c99
OMPFLAGS = -fopenmp



all: heat_serial heat_omp heat_mpi


serial_objects = heat_serial.o creategrid.o stepper.o boundaryconditions.o
heat_serial: $(serial_objects)
	$(CC) -o $@ $^ $(CFLAGS)


heat_omp: heat_omp.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $^ -o $@

heat_mpi: heat_mpi.c
	mpicc $(CFLAGSMPI) $^ -o $@ -lm

clean:
	$(RM) *.o
	$(RM) .depend

depend:
	$(CXX) -MM $(CXXFLAGS) *.cc > .depend

-include .depend






