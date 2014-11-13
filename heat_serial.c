#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "boundaryconditions.h"
#include "creategrid.h"
#include "stepper.h"
#include "pi.h"

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
  //  const float nx_double = atof(argv[1]);
  int check; /*used for checking outputs to make sure they are good*/
  double **T_arr; /*This will be a pointer to an array of pointers which will host the grid itself*/
  const double fraction_of_maximum_time_step = 0.8;/*This is the fraction of the largest numerically stable timestep, calculated below, that we want our dt to actually be.  Keeping it some sane fraction will allow us to get an exact fraction of the maximum time we want. In units of kappa*/
  const double dx = M_PI / (double)nx;
  const double dt =  dx * dx / 4.0 * fraction_of_maximum_time_step; /*This is the time step size, in units of kappa, which later cancel*/
  const int tmax = (int)(0.5 * M_PI * M_PI / dt);

  

  T_arr = grid_creator(nx);

  //Now initialize the array to the initial conditions
  //Our initial conditions are to have T=0 everywhere

  for(int i=0; i<nx; i++)
    {
      for(int j=0; j<nx; j++)
        {
          T_arr[i][j]=0.0;
        }
    }

  printf("%d\n",(int) (tmax/dt));
  for(int i=0; i<tmax; i++)
    {
      /*if (i%50 == 0)
        {
          printf("%d\n",i);
          }*/
      check = stepper(T_arr,nx,dx,dt);
      assert(check==0);
    }

  FILE *fp;

  if(!(fp=fopen("heat_serial.output.dat","w")))
    {
      printf("Output file isn't opening for saving.  Now quitting...\n");
      grid_destroyer(T_arr,nx);
      exit(1);
    }

  fprintf(fp,"#Final temperature stored in grid format\n");
  fprintf(fp,"#Each column in this file corresponds to column of actual grid, as does each row\n");
  
  for(int i=0; i<nx; i++)
    {
      for(int j=0; j<nx; j++)
        {
          fprintf(fp,"%e     ", T_arr[i][j]);
        }
      fprintf(fp,"\n");
    }
  fclose(fp);

  grid_destroyer(T_arr,nx);



  return 0;
}
