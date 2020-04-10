#ifndef IIR_H
#define IIR_H
#include "define.h"

absorp iirTest(char* filename);
absorp IIR (absorp my, float ** tableau);
float** create_tableau_IIR();
void supprime_tableau_IIR(float** tableau);

#endif