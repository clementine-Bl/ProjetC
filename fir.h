#ifndef FIR_H
#define FIR_H
#include "define.h"

absorp firTest(char* filename);
absorp FIR(absorp valueAbsorp, int *compteur_valeur, float** tableau);
float** create_tableau_FIR();
void supprime_tableau_FIR(float** tableau);
float FIR_TAPS[51]; // tableau des coefficients de h

#endif