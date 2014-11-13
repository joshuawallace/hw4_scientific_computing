#include <math.h>

#define M_PI acos(-1.0)

double T_x_0_boundaryconditions(int xi)
{
  /*This is the boundary condition along the "bottom" of the grid, where y=0*/
  /*xi is the index of x*/
  return cos((double)xi/M_PI) * cos((double)xi/M_PI);
}

double T_x_pi_boundaryconditions(int xi)
{
  /*This is the boundary condition along the "top" of the grid, where y=pi*/
  /*xi is the index of x*/
  return sin((double)xi/M_PI) * sin((double)xi/M_PI);
}

/*double T_0_y_boundaryconditions(int yi, int nx, double *T_arr)
{
  /*This is the boundary condition along the "left" of the grid, where x=0*/
  /*yi is the index of y*/
  /*nx is the integer number of elements on the side of an array*/
  /* **T_column is a pointer to the pointers that correspond to the whole array.  From this, the periodic boundary condition can be computed */

/*  return T_arr[nx-1];
}


double T_pi_y_boundaryconditions(int yi, double *T_arr)
{
  /*This is the boundary condition along the "right" of the grid, where x=pi*/
  /*yi is the index of y*/
  /* **T_column is a pointer to the pointers that correspond to the whole array.  From this, the periodic boundary condition can be computed */

/*  return T_arr[0];
    }*/
