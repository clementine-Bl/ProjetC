#include <stdio.h>
#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "mesure.h"
#include "integration.h"
#include "lecture.h"
#include "fichiers.h"

int main() {
    int file_state =0;
    int i=0;
    absorp myAbsorp;
    FILE* file_pf=initFichier("record1_bin.dat");
    while(file_state!=EOF) {
        myAbsorp = lecture(file_pf, &file_state);
        i++;
        printf("%f et %f et %f et %f\n", myAbsorp.acr, myAbsorp.dcr, myAbsorp.acir, myAbsorp.dcir);
    }

    /*absorp valeur =iirTest("record1_fir.dat");
    printf("%f,%f,%f,%f\n", valeur.acr,valeur.dcr,valeur.acir,valeur.dcir);
    absorp valeur2 =firTest("record1.dat");
    printf("%f,%f,%f,%f\n", valeur2.acr,valeur2.dcr,valeur2.acir,valeur2.dcir);
    oxy mesure1=mesureTest("record1_iir.dat");
    printf("%d,%d\n", mesure1.spo2, mesure1.pouls);
    affichage(mesure1);*/
    //integrationTest("record1.dat");
    return 0;
}