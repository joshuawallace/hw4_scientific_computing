#include <math.h>
#include <stdio.h>
#include "pi.h"

/*This calculates the y=0 and y=pi boundary conditions for the serial code*/

double T_x_0_boundaryconditions(int xi, int nx)
{
  /*This is the boundary condition along the "bottom" of the grid, where y=0*/
  /*xi is the index of x*/

  return cos(((double)xi + 0.5)/((double)nx) * M_PI) * cos(((double)xi + 0.5)/((double)nx) * M_PI);
}

double T_x_pi_boundaryconditions(int xi, int nx)
{
  /*This is the boundary condition along the "top" of the grid, where y=pi*/
  /*xi is the index of x*/

  return sin(((double)xi + 0.5)/((double)nx) * M_PI) * sin(((double)xi + 0.5)/((double)nx) * M_PI);
}
