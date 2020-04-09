#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "fichiers.h"
#include "integration.h"
#include "mesure.h"


void integrationTest(char* filename){
    int etat = 0;
    int cpt = 0;
    // on crée les tableau contenant les variables utiles pour les fonction FIR, IIR et MESURE
    float ** tab_FIR=create_tableau_FIR();
    float ** tab_IIR=create_tableau_IIR();
    float * tab_mesure=create_tableau_mesure();
    FILE* fichier = initFichier(filename);
    absorp signal;
    oxy valeur;
    valeur.pouls =0;  // initialisation
    valeur.spo2 =0;  //initialisation
    signal=lireFichier(fichier, &etat);
    while(etat != EOF){  // on analyse les données tant que le fichier n'est pas fini
        signal=FIR(signal,&cpt,tab_FIR);  //on filtre les donnes avec FIR
        signal=IIR(signal,tab_IIR);  //on filtre les donnes avec IIR
        valeur=MESURE(signal,tab_mesure,valeur);  //on calcul le poul et spo2 avec les donnes filtré
        affichage(valeur);  //on affiche les valeurs dans un fichier
        signal=lireFichier(fichier, &etat);  //on lit une nouvelle donnee
    }
    finFichier(fichier);
    // on supprime les tableaus de variables pour liberer l'espace memoire
    supprime_tableau_FIR(tab_FIR);
    supprime_tableau_IIR(tab_IIR);
    supprime_tableau_mesure(tab_mesure);
}