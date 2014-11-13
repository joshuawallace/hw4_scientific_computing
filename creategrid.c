#include <stdlib.h>
#include <stdio.h>
#include "creategrid.h"


double **grid_creator(const int nx)
{
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
