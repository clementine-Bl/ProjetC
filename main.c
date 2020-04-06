#include <stdio.h>
#include "affichage.h"
#include "fichiers.h"

int main() {
    int etat=0;
    absorp myAbsorp;
    printf("coucou\n");
    affichage(100,5);
    FILE* myFile = initFichier("record1.dat");
    myAbsorp = lireFichier(myFile,&etat);
    return 0;
}