#include "iir.h"
#include "fichiers.h"

absorp iirTest(char* filename){
    absorp myAbsorp, new ;  // creation d'un newAbsorp pour eviter de renvoyer que des 0 quand notre etat passe à EOF
    int etat=0;
    FILE* fichier=initFichier(filename);
    myAbsorp=lireFichier(fichier,&etat);
    float** parametre_IIR=create_tableau_IIR();  // creation de notre tableau de variables pour IIR
    do{  //tant qu'on est pas arrivé a la fin du fichier on lit les valeur et on les traite
        new=IIR(myAbsorp, parametre_IIR);
        myAbsorp=lireFichier(fichier,&etat);
    }while(etat != EOF);
    finFichier(fichier);
    supprime_tableau_IIR(parametre_IIR); // libère l'espace memoir pris par notre tableau
    return new;
}

absorp IIR (absorp my, float ** tableau){
    float y1r = my.acr-tableau[0][0]+0.992*tableau[0][1]; // calcul de y[n] de ac_r
    tableau[0][0]=my.acr; // remplace x[n-1] par x[n] de ac_r
    my.acr=y1r; // on modifie le acr de absorp par notre nouvelle valeur
    float y1ir = my.acir-tableau[1][0]+0.992*tableau[1][1]; // calcul de y[n] de ac_ir
    tableau[1][0]=my.acir; // remplace x[n-1] par x[n] de ac_ir
    my.acir=y1ir; // on modifie le acir de absorp par notre nouvelle valeur
    tableau[0][1]=y1r;
    tableau[1][1]=y1ir; // Sur les deux dernière ligne on remplace nos y[n-1] de ac_r et ac_ir
    return my;
}

float** create_tableau_IIR(){
    /*Detail tableau:
     Tableau[0] ligne de ac_r
        - Tableau[0][0] correspond a : x[n-1]
        - Tableau[0][1] correspond a : y[n-1]

     Tableau[1] ligne de ac_ir
        - Tableau[1][0] correspond a : x[n-1]
        - Tableau[1][1] correspond a : y[n-1]
     Fin détail tableau */
    float** tableau;
    int i;
    tableau=malloc(2* sizeof(float*));
    if (tableau != NULL){
        tableau[0]=malloc(2* sizeof(float));
        tableau[1]=malloc(2* sizeof(float));
        if (tableau[0] != NULL && tableau[1] != NULL) {
            for (i=0;i<2;i++) { // on initialise tout à 0 car au début nous n'avons pas les valeurs
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