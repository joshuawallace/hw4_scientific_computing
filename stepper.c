#include <stdlib.h>
#include <stdio.h>
#include "stepper.h"
#include "boundaryconditions.h"
#include "creategrid.h"



int stepper(double **T, const int nx, const double dx, const double dt)
{
  //double adjacent[4];//goes top, right, bottom, left


  double **deltaT = grid_creator(nx);
  
  for(int i=0; i<nx; i++)//which row, y
    {
      for(int j=0; j<nx; j++)//which column, x
        {
          double *adjacent = malloc(4 * sizeof(double));
          if(adjacent == NULL)
            {
              fprintf(stderr, "Malloc did not work.  Now exiting...\n");
              free(deltaT);
              exit(1);
            }
          
          
          if(i==0) //corresponds to the top
            {
              adjacent[0] = T_x_pi_boundaryconditions(j,nx);
            }
          else
            {
              adjacent[0] = T[i-1][j];
            }

          if(j==nx-1) //corresponds to right side
            {
              //adjacent[1] = T_pi_y_boundaryconditions(i,T[i]);
              adjacent[1] = T[i][0];
            }
          else
            {
              adjacent[1] = T[i][j+1];
            }

          if(i==nx-1) //corresponds to the bottom
            {
              adjacent[2] = T_x_0_boundaryconditions(j,nx);
            }
          else
            {
              adjacent[2] = T[i+1][j];
            }

          if(j==0) //corresponds to left side
            {
              //adjacent[3] = T_0_y_boundaryconditions(i,T[i]);
              adjacent[3] = T[i][nx-1];
            }
          else
            {
              adjacent[3] = T[i][j-1];
            }

          
          /*for(int k=0; k<4; k++)
            {
              if(adjacent[k] == NULL)
                {
                  printf("Your stepper calculation had a malfunction.\n");
                  grid_destroyer(deltaT,nx);
                  free(adjacent);
                  return -1;
                }
                }*/


          deltaT[i][j] = dt / (dx * dx) * (adjacent[0] + adjacent[1] + adjacent[2] + adjacent[3] - 4.*T[i][j]);
          free(adjacent);
        }
    }


  for(int i=0; i<nx; i++)//which row, y
    {
      for(int j=0; j<nx; j++)//which column, x
        {
          T[i][j] += deltaT[i][j];
        }
    }
  grid_destroyer(deltaT,nx);

  return 0;
}
