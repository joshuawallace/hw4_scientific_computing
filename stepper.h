#ifndef STEPPER_H_
#define STEPPER_H_

/*Header file for stepper.c, which steps the algorithm forward in time*/

int stepper(double **T, double **T2, const int nx, const double dx, const double dt);

#endif
