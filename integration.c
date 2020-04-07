#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "fichiers.h"
#include "integration.h"
#include "mesure.h"


void integrationTest(char* filename){
    int etat = 0;
    int cpt = 0;
    float ** tab_FIR=create_tableau_FIR();
    float ** tab_IIR=create_tableau_IIR();
    float * tab_mesure=create_tableau_mesure();
    FILE* fichier = initFichier(filename);
    absorp signal;
    oxy valeur;
    valeur.pouls =0;
    valeur.spo2 =0;
    signal=lireFichier(fichier, &etat);
    while(etat != EOF){
        signal=FIR(signal,&cpt,tab_FIR);
        signal=IIR(signal,tab_IIR);
        valeur=MESURE(signal,tab_mesure,valeur);
        affichage(valeur);
        signal=lireFichier(fichier, &etat);
    }
    finFichier(fichier);
    supprime_tableau_FIR(tab_FIR);
    supprime_tableau_IIR(tab_IIR);
    supprime_tableau_mesure(tab_mesure);
}