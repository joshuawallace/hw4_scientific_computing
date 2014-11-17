#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "mpi.h"

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

double **grid_creator(const int nx, const int n)
{
  double **pointer;

  pointer = malloc(nx * sizeof(double *));
  if(pointer == NULL)//if the memory wasn't allocated
    {
      fprintf(stderr, "Malloc did not work.  Now exiting...\n");
      MPI_Finalize();
      exit(1);
    }

  for(int i = 0; i < nx; i++)
    {
      pointer[i] = malloc(n * sizeof(double));
      if(pointer[i] == NULL)//if the memory for a particular row wasn't allocated
        {
          fprintf(stderr, "Malloc did not work.  Now exiting...\n");
          for(int j=0; j < i; j++)
            {
              free(pointer[j]);//free all the previous successfully allocated rows
            }
          free(pointer[i]);//free the array of pointers
          MPI_Finalize();
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


int stepper(double **T, double **T2, const int nx, const double dx, const double dt, const int ncols, const int rank)
{
 for(int i=0; i<nx; i++)//which row, y
    {
      for(int j=1; j<(ncols+2-1); j++)//which column, x
        {
          double *adjacent = malloc(4 * sizeof(double));
          if(adjacent == NULL)
            {
              fprintf(stderr, "Malloc did not work.  Now exiting...\n");
              MPI_Finalize();
              exit(1);
            }
          
          
          if(i==0) //corresponds to the top
            {
              adjacent[0] = T_x_pi_boundaryconditions((rank * ncols + j),nx);
            }
          else
            {
              adjacent[0] = T[i-1][j];
                }
          
          /*if(j==ncols-1) //corresponds to right side
            {
              adjacent[1] = T[i][0];
            }
          else
            {
              adjacent[1] = T[i][j+1];
              }*/
          adjacent[1] = T[i][j+1];
          
          if(i==nx-1) //corresponds to the bottom
                {
                  adjacent[2] = T_x_0_boundaryconditions((rank * ncols + j),nx);
                }
          else
            {
              adjacent[2] = T[i+1][j];
            }
          
          /*if(j==0) //corresponds to left side
            {
              adjacent[3] = T[i][nx-1];
            }
          else
            {
              adjacent[3] = T[i][j-1];
            }*/
          adjacent[3] = T[i][j-1];

          T2[i][j] = T[i][j] + dt / (dx * dx) * (adjacent[0] + adjacent[1] + adjacent[2] + adjacent[3] - 4.*T[i][j]);
          free(adjacent);
        }
    }

  return 0;
}

void  initial_message(char *name)
{
  printf("Usage: %s <nx>  \n",name);
  printf("  nx:    grid size on a side\n         final grid will be 2-d sized nx by nx\n");
  exit(1);
}

int main(int argc, char *argv[]) 
{

  double start_time = MPI_Wtime();

  
  if (argc!=2) 
    {
      initial_message(argv[0]);
    }

  int rc;
  rc = MPI_Init(&argc,&argv);
  if( rc != MPI_SUCCESS) {
    printf("Error starting MPI program.  Now quitting\n");
    MPI_Abort(MPI_COMM_WORLD,rc);
  }

  int numtasks,rank;

  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  
  
  
  const int nx = atoi(argv[1]); 
  

  if(nx % numtasks !=0)
    {
      printf("Your chosen nx does not divide nicely into your chosen number of threads.\n Change the code or your arguments.  Now quitting...\n");
      MPI_Finalize();
      exit(0);
    }

  const int ncols = nx/numtasks;

  MPI_Request reqs[4];
  MPI_Status stats[4];
  MPI_Status stats2[2];



  int check; /*used for checking outputs to make sure they are good*/
  double **T_arr; /*This will be a pointer to an array of pointers which will host the grid itself*/
  double **T_arr_2; /*This will be used to calculate the new T values at each time step while keeping the old values in T_arr for calculation purposes*/
  double **T_pointer_temp; /*To be used to switch the pointers T_arr and T_arr_2*/
  const double fraction_of_maximum_time_step = 0.8;/*This is the fraction of the largest numerically stable timestep, calculated below, that we want our dt to actually be.  Keeping it some sane fraction will allow us to get an exact fraction of the maximum time we want. In units of kappa*/
  const double dx = M_PI / (double)nx;
  const double dt =  dx * dx / 4.0 * fraction_of_maximum_time_step; /*This is the time step size, in units of kappa, which later cancel*/
  const double tmax = (0.5 * M_PI * M_PI);
  const int ntstep = (int) (tmax/dt);

  const int prev = (rank==0) ? numtasks-1 : rank-1;
  const int next = (rank==(numtasks - 1)) ? 0 : rank+1;

  const int tag1=1;
  const int tag2=2;
  


  T_arr = grid_creator(nx,(ncols+2));
  T_arr_2 = grid_creator(nx,(ncols+2));

  //Now initialize the array to the initial conditions
  //Our initial conditions are to have T=0 everywhere

  for(int i=0; i<nx; i++)
    {
      for(int j=1; j<(ncols+2-1); j++)
        {
          T_arr[i][j]=0.0;
        }
    }

  //printf("%d\n",(int) (tmax/dt));
  for(int i=0; i<ntstep; i++)
    {
      //pass the boundary columns between the various threads
      double right_pass[nx],left_pass[nx],right_accept[nx],left_accept[nx];

      for(int l=0; l<nx; l++)
        {
          left_pass[l]=T_arr[l][1];
          right_pass[l] =T_arr[l][ncols-2];
	  if(i==0)
	    {
	      if(left_pass[l] > 1.e-12)
		printf("Offending leftpass: index %d,  rank %d\n value: %e",l,rank,left_pass[l]);
	      if(right_pass[l] > 1.e-12)
		printf("Offending rightpass: index %d,  rank %d\n value: %e",l,rank,right_pass[l]);
	    }
        }


      //printf("Made it here %d\n",rank);
      MPI_Isend(&left_pass, nx, MPI_DOUBLE, prev, tag1, MPI_COMM_WORLD, &reqs[0]);
      //printf("First send %d\n",rank);
      MPI_Isend(&right_pass, nx, MPI_DOUBLE, next, tag2, MPI_COMM_WORLD, &reqs[1]);
      //printf("Second send %d\n",rank);

      MPI_Irecv(&left_accept, nx, MPI_DOUBLE, prev, tag2, MPI_COMM_WORLD, &reqs[2]);
      //printf("First receive %d\n",rank);
      //fflush(stdout);
      MPI_Irecv(&right_accept, nx, MPI_DOUBLE, next, tag1, MPI_COMM_WORLD, &reqs[3]);
      //printf("Second receive %d\n",rank);
      //fflush(stdout);

      MPI_Waitall(4, reqs, stats);
      //printf("Wait all done %d\n",rank);
      //fflush(stdout);

      for(int l=0; l<nx; l++)
        {
	  //printf("%d    %d\n",i,rank);
          T_arr[l][0] = left_accept[l];
          T_arr[l][ncols+2-1] = right_accept[l];
	  if(i==0)
	    {
	      if(left_accept[l] > 1.e-12)
		printf("Offending leftaccept: index %d,  rank %d\n value: %e",l,rank,left_accept[l]);
	      if(right_accept[l] > 1.e-12)
		printf("Offending rightaccept: index %d,  rank %d\n value: %e",l,rank,right_accept[l]);
	    }
        }
      if(i==0)
	{
	  for(int j=0; j<nx; j++)
	    {
	      for(int k=0; k<(ncols+2); k++)
		{
		  if( (T_arr[j][k] - 0.0) > 1.e-12)
		    {
		      printf("Offending T: index %d %d,  rank %d\n value: %e",j,k,rank,T_arr[j][k]);
		    }
		}
	    }
	}
      
      if (i == 10)
      {
        printf("%d   %d\n",i,rank);
      }
      //printf("Step in  %d\n",rank);
      check = stepper(T_arr,T_arr_2,nx,dx,dt,ncols,rank);
      assert(check==0);
      //printf("step out   %d\n",rank);


      /*The following switches the pointers T_arr and T_arr_2, making T_arr now equal to the newly updated array formerly pointed to by T_arr_2 and giving the T_arr_2 pointer the old array*/
      T_pointer_temp = T_arr_2;
      T_arr_2 = T_arr;
      T_arr = T_pointer_temp;
    }



  FILE *fp;


  char outputfilename[120] = "heat_omp.";
  char stringtemp[120];
  sprintf(stringtemp, "%d", nx);
  strcat(outputfilename,stringtemp);
  strcat(outputfilename,".");
  sprintf(stringtemp, "%d", numtasks);
  strcat(outputfilename,stringtemp);
  strcat(outputfilename,".rank.");
  sprintf(stringtemp, "%d", rank);
  strcat(outputfilename,stringtemp);
  strcat(outputfilename,".output.dat");
  
  if(!(fp=fopen(outputfilename,"w")))
    {
      printf("Output file isn't opening for saving.  Now quitting...\n");
      grid_destroyer(T_arr,nx);
      grid_destroyer(T_arr_2,nx);
      MPI_Finalize();
      exit(1);
    }

  fprintf(fp,"#Final temperature stored in grid format, MPI style\n");
  fprintf(fp,"#Columns represent the rows in the actual grid and vice-versa.  This is so that each separate rank's file can easily be appended to the others to produce a full file.\n");
  
  for(int i=1; i<(ncols+2-1); i++)
    {
      for(int j=0; j<nx; j++)
        {
          fprintf(fp,"%e     ", T_arr[j][i]); //For a fixed column all the elements get printed horizontally in the file.
        }
      fprintf(fp,"\n");
    }
  fclose(fp);

  grid_destroyer(T_arr,nx);
  grid_destroyer(T_arr_2,nx);
  
  MPI_Finalize();

  printf("numtasks: %d, rank: %d, nx: %d, time: %lf\n",numtasks,rank,nx,MPI_Wtime()-start_time);

  return 0;
}
