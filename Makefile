CXXFLAGS = -g -Wall -std=c99
CFLAGS = -g -Wall -lm -std=c99
CFLAGSMPI = -g -Wall -std=c99
OMPFLAGS = -fopenmp



all: heat_serial heat_omp heat_mpi


serial_objects = heat_serial.o creategrid.o stepper.o boundaryconditions.o
heat_serial: $(serial_objects)
	$(CC) -o $@ $^ $(CFLAGS)

#heat_serial.e: heat_serial.e.o creategrid.o stepper.e.o boundaryconditions.o
#	$(CC) -o $@ $^ $(CFLAGS)
#omp_objects = heat_omp.o creategrid.o stepper.omp.o boundaryconditions.o
#heat_omp: $(omp_objects)
#	$(CC) -o $@ $^ $(OMPFLAGS) $(CFLAGS)

#heat_omp.together: heat_omp.together.o creategrid.o boundaryconditions.o
#	$(CC) -o $@ $^ $(OMPFLAGS) $(CFLAGS)

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






