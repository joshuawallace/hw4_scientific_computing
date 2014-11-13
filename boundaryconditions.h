#ifndef BC_H_
#define BC_H_



double T_x_0_boundaryconditions(int xi);
    
double T_x_pi_boundaryconditions(int xi);


double T_0_y_boundaryconditions(int yi, double *T_column);

double T_pi_y_boundaryconditions(int yi, double *T_column);

#endif
