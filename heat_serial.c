#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "boundaryconditions.h"
#include "creategrid.h"
#include "stepper.h"
#include "pi.h"

void  initial_message(char *name)
{
  /*This runs if there is a wrong number of input parameters*/
  printf("Usage: %s <nx>\n",name);
  printf("  nx:    grid size on a side\n         final grid will be 2-d sized nx by nx\n");
  exit(1);
}

int main(int argc, char *argv[]) 
{
  if (argc!=2) 
    {
      initial_message(argv[0]);
    }

  const int nx = atoi(argv[1]); //Array size
  int check; /*used for checking outputs to make sure they are good*/
  double **T_arr; /*This will be a pointer to an array of pointers which will host the grid itself*/
  double **T_arr_2; /*This will be used to calculate the new T values at each time step while keeping the old values in T_arr for calculation purposes*/
  double **T_pointer_temp; /*To be used to switch the pointers T_arr and T_arr_2*/
  const double fraction_of_maximum_time_step = 0.8;/*This is the fraction of the largest numerically stable timestep, calculated below, that we want our dt to actually be.  Keeping it some sane fraction will allow us to get an exact fraction of the maximum time we want. In units of kappa*/
  const double dx = M_PI / (double)nx; //physical size of each array cell
  const double dt =  dx * dx / 4.0 * fraction_of_maximum_time_step; /*This is the time step size, in units of kappa, which later cancel*/
  const double tmax = (0.5 * M_PI * M_PI); //maximum time
  const int ntstep = (int) (tmax/dt);//number of time steps

  

  T_arr = grid_creator(nx);//Create two arrays to store the temperatures.
  T_arr_2 = grid_creator(nx);

  //Now initialize the array to the initial conditions
  //Our initial conditions are to have T=0 everywhere

  for(int i=0; i<nx; i++)
    {
      for(int j=0; j<nx; j++)
        {
          T_arr[i][j]=0.0;
        }
    }

  for(unsigned long long int i=0; i<ntstep; i++)//ull int type in case we need a big run
    {
      if (i%5000 == 0)
      {
        printf("%llu\n",i);//prints out every so often to let us know the computer is still running
      }
      check = stepper(T_arr,T_arr_2,nx,dx,dt); //step forward one time step
      assert(check==0);


      /*The following switches the pointers T_arr and T_arr_2, making T_arr now equal to the newly updated array formerly pointed to by T_arr_2 and giving the T_arr_2 pointer the old array*/
      T_pointer_temp = T_arr_2;
      T_arr_2 = T_arr;
      T_arr = T_pointer_temp;
    }

  FILE *fp;


  char outputfilename[120] = "heat_serial."; //name of output file
  char stringtemp[120];
  sprintf(stringtemp, "%d", nx);
  strcat(outputfilename,stringtemp);
  strcat(outputfilename,".output.dat");
  
  if(!(fp=fopen(outputfilename,"w"))) //open the file
    {
      printf("Output file isn't opening for saving.  Now quitting...\n");
      grid_destroyer(T_arr,nx);
      grid_destroyer(T_arr_2,nx);
      exit(1);
    }

  fprintf(fp,"#Final temperature stored in grid format\n");
  fprintf(fp,"#Each column in this file corresponds to column of actual grid, as does each row\n");
  

  //print out the data
  for(int i=0; i<nx; i++)
    {
      for(int j=0; j<nx; j++)
        {
          fprintf(fp,"%e     ", T_arr[i][j]);
        }
      fprintf(fp,"\n");
    }
  fclose(fp);

  grid_destroyer(T_arr,nx); //free up the memory
  grid_destroyer(T_arr_2,nx);



  return 0;
}
