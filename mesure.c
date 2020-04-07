#include "mesure.h"
#include "fichiers.h"

oxy mesureTest(char* filename){
    float* tableau = create_tableau_mesure();
	oxy myOxy;
	myOxy.pouls =0; // initialisation avant la première
	myOxy.spo2 =0; // initialisation avant la première
	absorp myAbsorp;
    FILE* fichier = initFichier(filename);
    int etat =0;
    myAbsorp = lireFichier(fichier,&etat);
    while(etat != EOF) {
        myOxy = MESURE(myAbsorp, tableau,myOxy);
        myAbsorp = lireFichier(fichier, &etat);
    }
    finFichier(fichier);
    return myOxy;

}

float* create_tableau_mesure(){
    float* tableau;
    tableau =malloc(8* sizeof(float));
    if (tableau == NULL){
        printf("le tableau n'a pas pu être crée");
    }
    return tableau;
}

oxy MESURE(absorp myAbsorp, float* tableau,oxy myOxy){
    float ratio;

    /*Tableau[0] = min_ac_r
     Tableau[1] = max_ac_r
     Tableau[2] = min_ac_ir
     Tableau[3] = max_ac_ir
     Tableau[4] = compteur periode
     Tableau[5] = 1 ou 0 si la demi-periode est faite
     Tableau[6] = 1 croissant ou 0 decroissant sens du premier point de lecture
     Tableau[7] = valeur de début de periode*/
    if(tableau[4]==0){
        tableau[7] = myAbsorp.acr;
        tableau[0] = tableau[7]; // initialisation du min ac_r
        tableau[1]= tableau[7]; // initialisation du max ac_r
        tableau[2]= myAbsorp.acir; // initialisation du min ac_ir
        tableau[3]= myAbsorp.acir; // initialisation du max ac_ir
        tableau[4] = tableau[4] + 1;
        tableau[5]=0;
    }else{
        if(tableau[4]==1){

            if(myAbsorp.acr > tableau[7]){
                tableau[6] = 1;
            }else{
                tableau[6] = 0;
            }
            min_max(myAbsorp.acr,&tableau[0],&tableau[1]);
            min_max(myAbsorp.acir,&tableau[2],&tableau[3]);

            tableau[4] = tableau[4] + 1;
        }else{
            if(tableau[5] != 1){ // On fait tant qu'une demi periode n'est pas faite

                if((tableau[6] == 1 && myAbsorp.acr < tableau[7]) || (tableau[6] == 0 && myAbsorp.acr > tableau[7])){

                    /* Si le debut commençait de façon croissante et que notre nouvelle valeur est plus petite que notre valeur de départ
                     une demi periode est passé*/
                    /* Ou bien si le debut commençait de façon décroissante et que notre nouvelle valeur est plus grande que notre valeur de départ
                     une demi periode est passé*/
                    tableau[5]=1;
                }
                min_max(myAbsorp.acr,&tableau[0],&tableau[1]);
                min_max(myAbsorp.acir,&tableau[2],&tableau[3]);
                tableau[4] = tableau[4] + 1;
            }else {
                if (tableau[5] == 1) {
                    if((tableau[6] == 1 && myAbsorp.acr > tableau[7]) || (tableau[6] == 0 && myAbsorp.acr < tableau[7])) {
                        /*Une demi periode est déjà passé
                         Si le debut commençait de façon croissante et que notre nouvelle valeur est plus grande que notre valeur de départ
                         une periode est passé*/
                        /* Ou bien si le debut commençait de façon décroissante et que notre nouvelle valeur est plus petite que notre valeur de départ
                         une periode est passé*/
                        ratio = ((tableau[1] - tableau[0]) / myAbsorp.dcr) / ((tableau[3] - tableau[2]) / myAbsorp.dcir);
                        /* Calcul du ratio : Tableau[1]-Tableau[0] : max-min amplitude crête à crête de ac_r
                         Tableau[3]-Tableau[2] : max-min amplitude crête à crête de ac_ir*/
                        if (ratio <= 1 && ratio >= 0.4) {
                            myOxy.spo2 = (-25) * ratio + 110;
                        } else {
                            if(ratio <=3.4 && ratio > 1) {
                                myOxy.spo2 = (-35.7) * ratio + 121.38;
                            }
                        }
                        myOxy.pouls = 30000 / tableau[4];
                        tableau[4] = 0;

                    }else {
                        min_max(myAbsorp.acr, &tableau[0], &tableau[1]);
                        min_max(myAbsorp.acir, &tableau[2], &tableau[3]);
                        tableau[4] = tableau[4] + 1;
                    }
                }

            }
        }
    }
    return myOxy;
}


void min_max(float value,float* min, float* max){
    if(value < *min){ // mise à jour de min ac_ir
        *min = value;
    }else {
        if (value > *max) { // mise à jour de max ac_ir que si myAbsorb n'est pas déjà un min
            *max = value;
        }
    }
}

void supprime_tableau_mesure(float* tableau){
    free(tableau);
}