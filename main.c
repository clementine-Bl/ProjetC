#include <stdio.h>
#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "mesure.h"
#include "integration.h"
#include "lecture.h"
#include "fichiers.h"

int main() {

    // permet de tester lecture
    int file_state =0;
    int i=0;
    absorp myAbsorp;
    FILE* file_pf=initFichier("record1_bin.dat");
    while(file_state!=EOF) {
        myAbsorp = lecture(file_pf, &file_state);
        i++;
    }
    finFichier(file_pf);
    printf("%f,%f,%f,%f\n", myAbsorp.acr, myAbsorp.dcr, myAbsorp.acir, myAbsorp.dcir);

    //permet de tester FIR
    absorp valeur2 =firTest("record1.dat");
    printf("%f,%f,%f,%f\n", valeur2.acr,valeur2.dcr,valeur2.acir,valeur2.dcir);

    //permet de tester IIR
    absorp valeur =iirTest("record1_fir.dat");
    printf("%f,%f,%f,%f\n", valeur.acr,valeur.dcr,valeur.acir,valeur.dcir);

    //permet de tester mesure et affichage
    oxy mesure1=mesureTest("record1_iir.dat");
    printf("%d,%d\n", mesure1.spo2, mesure1.pouls);
    affichage(mesure1);

    //permet de tester integration
    integrationTest("record1.dat");

    return 0;
}