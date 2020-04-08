#include "iir.h"
#include "fichiers.h"

absorp iirTest(char* filename){
    absorp myAbsorp, new ;
    int etat=0;
    FILE* fichier=initFichier(filename);
    myAbsorp=lireFichier(fichier,&etat);
    float** parametre_IIR=create_tableau_IIR();
    do{
        new=IIR(myAbsorp, parametre_IIR);
        myAbsorp=lireFichier(fichier,&etat);
    }while(etat != EOF);
    finFichier(fichier);
    supprime_tableau_IIR(parametre_IIR);
    return new;
}

absorp IIR (absorp my, float ** tableau){
    float y1r = my.acr-tableau[0][0]+0.992*tableau[0][1];
    tableau[0][0]=my.acr;
    my.acr=y1r;
    float y1ir = my.acir-tableau[1][0]+0.992*tableau[1][1];
    tableau[1][0]=my.acir;
    my.acir=y1ir;
    tableau[0][1]=y1r;
    tableau[1][1]=y1ir;
    return my;
}

float** create_tableau_IIR(){
    float** tableau;
    int i;
    tableau=malloc(2* sizeof(float*));
    if (tableau != NULL){
        tableau[0]=malloc(2* sizeof(float));
        tableau[1]=malloc(2* sizeof(float));
        if (tableau[0] != NULL && tableau[1] != NULL) {
            for (i=0;i<2;i++) {
                tableau[0][i] = 0;
                tableau[1][i] = 0;
            }
        }
    }else{
        printf("le tableau n'a pas pu être crée");
    }
    return tableau;
}

void supprime_tableau_IIR(float** tableau){
    free(tableau[0]);
    free(tableau[1]);
    free(tableau);
}