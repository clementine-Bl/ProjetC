#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "fichiers.h"
#include "integration.h"


void integrationTest(char* filename){
    int etat = 0;
    int cpt = 0;
    float ** tab_FIR=create_tableau_FIR();
    float ** tab_IIR=create_tableau_IIR();
    FILE* fichier = initFichier("record1.dat");
    absorp signal;
    oxy valeur;
    while(etat != EOF){
        signal=lireFichier(fichier, &etat);
        signal=FIR(signal,&cpt,tab_FIR);
        signal=IIR(signal,tab_IIR);

    }
}