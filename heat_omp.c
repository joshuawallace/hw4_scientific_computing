#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <omp.h>

#ifndef M_PI
#define M_PI  acos(-1.0)

#endif


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

double **grid_creator(const int nx)
{
  //Create a space in the memory to store the temperature as we solve for it

  double **pointer;

  pointer = malloc(nx * sizeof(double *));
  if(pointer == NULL)//if the memory wasn't allocated
    {
      fprintf(stderr, "Malloc did not work.  Now exiting...\n");
      exit(1);
    }

  for(int i = 0; i < nx; i++)
    {
      pointer[i] = malloc(nx * sizeof(double));
      if(pointer[i] == NULL)//if the memory for a particular row wasn't allocated
        {
          fprintf(stderr, "Malloc did not work.  Now exiting...\n");
          for(int j=0; j < i; j++)
            {
              free(pointer[j]);//free all the previous successfully allocated rows
            }
          free(pointer[i]);//free the array of pointers
          exit(1);
        }
    }

  return pointer;
}

void grid_destroyer(double **pointer, const int nx)
{
  //free memory at the end
  for(int i=0; i < nx; i ++)
    {
      free(pointer[i]);
    }
  free(pointer);
}

int stepper(double **T, double **T2, const int nx, const double dx, const double dt, int nthread)
{
  /*This steps the algorithm forward one step in time*/

  omp_set_num_threads(nthread);  //Number of threads
#pragma omp parallel for
 for(int i=0; i<nx; i++)//which row, y
    {
      for(int j=0; j<nx; j++)//which column, x
        {
          double *adjacent = malloc(4 * sizeof(double));
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
              adjacent[1] = T[i][0];  //read from left side, periodic boundary condition
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
              adjacent[3] = T[i][nx-1]; //read from right side, periodic boundary condition
            }
          else
            {
              adjacent[3] = T[i][j-1];
            }


          T2[i][j] = T[i][j] + dt / (dx * dx) * (adjacent[0] + adjacent[1] + adjacent[2] + adjacent[3] - 4.*T[i][j]); //calculate new temperature
          free(adjacent);
        }
    }
 

  return 0;
}

void  initial_message(char *name)
{
  /*This runs if the incorrect number of command line arguments are given*/
  
  printf("Usage: %s <nx>  <nthread>\n",name);
  printf("  nx:    grid size on a side\n         final grid will be 2-d sized nx by nx\n nthread:   number of threads desired for parallel computing\n");
  exit(1);
}

int main(int argc, char *argv[]) 
{

  double start_time = omp_get_wtime(); //let's time how long this run takes
  if (argc!=3) 
    {
      initial_message(argv[0]);
    }
  
  const int nx = atoi(argv[1]); //grid size
  const int nthread = atoi(argv[2]); //number of threads for OMP
  if(nthread<1)
    {
      printf("Make sure you define nthread to be AT LEAST 1\n Now exiting...\n");
      exit(0);
    }

 
  int check; /*used for checking outputs to make sure they are good*/
  double **T_arr; /*This will be a pointer to an array of pointers which will host the grid itself*/
  double **T_arr_2; /*This will be used to calculate the new T values at each time step while keeping the old values in T_arr for calculation purposes*/
  double **T_pointer_temp; /*To be used to switch the pointers T_arr and T_arr_2*/
  const double fraction_of_maximum_time_step = 0.8;/*This is the fraction of the largest numerically stable timestep, calculated below, that we want our dt to actually be.  Keeping it some sane fraction will allow us to get an exact fraction of the maximum time we want. In units of kappa*/
  const double dx = M_PI / (double)nx; //physical size of cell
  const double dt =  dx * dx / 4.0 * fraction_of_maximum_time_step; /*This is the time step size, in units of kappa, which later cancel*/
  const double tmax = (0.5 * M_PI * M_PI); //maximum time to go
  const int ntstep = (int) (tmax/dt);  //number of time steps

  

  T_arr = grid_creator(nx); //allocate memory for the temperature and its companion array
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

  printf("%d\n",(int) (tmax/dt));
  for(int i=0; i<ntstep; i++)//for each timestep
    {
      check = stepper(T_arr,T_arr_2,nx,dx,dt,nthread); //step forward in time
      assert(check==0);


      /*The following switches the pointers T_arr and T_arr_2, making T_arr now equal to the newly updated array formerly pointed to by T_arr_2 and giving the T_arr_2 pointer the old array*/
      T_pointer_temp = T_arr_2;
      T_arr_2 = T_arr;
      T_arr = T_pointer_temp;
    }

  FILE *fp;


  char outputfilename[120] = "heat_omp."; //save file name
  char stringtemp[120];
  sprintf(stringtemp, "%d", nx);
  strcat(outputfilename,stringtemp);
  strcat(outputfilename,".output.dat");
  
  if(!(fp=fopen(outputfilename,"w")))
    {
      printf("Output file isn't opening for saving.  Now quitting...\n");
      grid_destroyer(T_arr,nx);
      grid_destroyer(T_arr_2,nx);
      exit(1);
    }

  fprintf(fp,"#Final temperature stored in grid format\n");
  fprintf(fp,"#Each column in this file corresponds to column of actual grid, as does each row\n");
  

  //print data to the file
  for(int i=0; i<nx; i++)
    {
      for(int j=0; j<nx; j++)
        {
          fprintf(fp,"%e     ", T_arr[i][j]);
        }
      fprintf(fp,"\n");
    }
  fclose(fp);


  //free memory
  grid_destroyer(T_arr,nx);
  grid_destroyer(T_arr_2,nx);

  //print the time it took to standard output
  printf("threads: %d, nx: %d, time: %lf\n",nthread,nx,omp_get_wtime()-start_time);

  return 0;
}
