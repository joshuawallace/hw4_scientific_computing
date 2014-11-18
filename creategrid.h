#ifndef GRIDCREATOR_H_
#define GRIDCREATOR_H_

/*Headers file for creategrid.c, which creates and destroys the arrays to hold the temperatures for the calculation*/

double **grid_creator(const int nx);

void grid_destroyer(double **pointer, const int nx);

#endif
