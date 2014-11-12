#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "boundaryconditions.h"

void  initial_message(char *name)
{
  printf("Usage: %s <nx>\n",name);
  printf("  nx:    grid size on a side\n         final grid will be 2-d sized nx by nx\n");
  exit(1);
}

int main(int argc, char *argv[]) 
{
  if (argc!=2) {
    initial_message(argv[0]);
  }

  const int nx = atoi(argv[1]); 
  int check; //used for checking outputs to make sure they are good
  double **T_arr; //This will be a pointer to an array of pointers which will host the grid itself
  const double fraction_of_maximum_time_step=0.1;//This is the fraction of the largest numerically stable timestep, calculated below, that we want our dt to actually be.  Keeping it some sane fraction will allow us to get an exact fraction of the maximum time we want. In units of kappa
  const double dt = N_PI * N_PI / (4.0 * (double)nx * (double)nx) * fraction_of_maximum_time_step; //This is the time step size, in units of kappa, which later cancel
  

  T_arr = malloc(nx * sizeof(double *));
  if(T_arr = NULL)//if the memory wasn't allocated
    {
      fprintf(stderr, "Malloc did not work.  Now exiting...\n");
      exit(1);
    }

  for(int i = 0; i < nx; i++)
    {
      T_arr[i] = malloc(nx * sizeof(double));
      if(T_arr[i] = NULL)//if the memory for a particular row wasn't allocated
        {
          fprintf(stderr, "Malloc did not work.  Now exiting...\n");
          for(int j=0; j < i; j++)
            {
              free(T_arr[j]);//free all the previous successfully allocated rows
            }
          free(T_arr[i]);//free the array of pointers
          exit(1);
        }
    }

  //Now initialize the array to the initial conditions
  //Our initial conditions are to have T=0 everywhere

  for(int i=0; i<nx; i++)
    {
      for(int j=0; j<nx; j++)
        {
          T_arr[i][j]=0.0;
        }
    }




  //free memory at the end
  for(int i=0; i < nx; i ++)
    {
      free(T_arr[i]);
    }
  free(T_arr)


  return 0;
}
