#include <stdlib.h>
#include <stdio.h>
#include "stepper.h"
#include "boundaryconditions.h"
#include "creategrid.h"



int stepper(double **T, double **T2, const int nx, const double dx, const double dt)
{

  for(int i=0; i<nx; i++)//which row, y
    {
      for(int j=0; j<nx; j++)//which column, x
        {
          double *adjacent = malloc(4 * sizeof(double)); //array to store adjacent cell values for temperature calculation.

          if(adjacent == NULL)
            {
              fprintf(stderr, "Malloc did not work.  Now exiting...\n");
               exit(1);
            }
          
          
          if(i==0) //corresponds to the top
            {
              adjacent[0] = T_x_pi_boundaryconditions(j,nx); //top boundary condition
            }
          else
            {
              adjacent[0] = T[i-1][j];
            }

          if(j==nx-1) //corresponds to right side
            {
              adjacent[1] = T[i][0]; //get T from left side, periodic boundary condition
            }
          else
            {
              adjacent[1] = T[i][j+1];
            }

          if(i==nx-1) //corresponds to the bottom
            {
              adjacent[2] = T_x_0_boundaryconditions(j,nx); //bottom boundary condition
            }
          else
            {
              adjacent[2] = T[i+1][j];
            }

          if(j==0) //corresponds to left side
            {
              adjacent[3] = T[i][nx-1];  //read from right side, periodic boundary condition
            }
          else
            {
              adjacent[3] = T[i][j-1];
            }


          T2[i][j] = T[i][j] + dt / (dx * dx) * (adjacent[0] + adjacent[1] + adjacent[2] + adjacent[3] - 4.*T[i][j]); //calculate new T value
          free(adjacent);
        }
    }

  return 0;
}
