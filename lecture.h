#ifndef LECTURE_H
#define LECTURE_H
#include "define.h"

absorp lecture(FILE* file_pf, int* file_state);
float additoner (int millier, int centaine,int dizaine, int unite);
absorp modifier  (absorp my, int compteur, float valeur);
void test_fin_fichier(int* octet,int* valid,int* file_state);
#endif