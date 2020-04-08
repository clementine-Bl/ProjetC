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
    supprime_tableau_mesure(tableau);
    return myOxy;

}

float* create_tableau_mesure(){
    /* On crée un tableau pour stocker les variables dont on a besoin à chaque appelle de mesure
     Tableau[0] = min_ac_r
     Tableau[1] = max_ac_r
     Tableau[2] = min_ac_ir
     Tableau[3] = max_ac_ir
     Tableau[4] = compteur valeur pendant une periode
     Tableau[5] = 1 ou 0 si la demi-periode est faite
     Tableau[6] = 1 croissant ou 0 decroissant sens du premier point de lecture
     Tableau[7] = valeur de début de periode
     Tableau[8] = valeur du compteur valeur pendant une periode n-1
     Tableau[9] = valeur du compteur valeur pendant une periode n-2*/
    float* tableau;
    int i;
    tableau =malloc(10* sizeof(float));  //on alloue 10 espace memoire
    if (tableau != NULL) {
        for (i=0;i<10;i++){
            tableau[i]=0;  //on initialise chaque element à 0
        }
    }else{
        printf("le tableau n'a pas pu être crée");
    }
    return tableau;
}

oxy MESURE(absorp myAbsorp, float* tableau,oxy myOxy){
    float ratio;
    if(tableau[4]==0){
        tableau[7] = myAbsorp.acr;  //la première valeur va servir de valeur de debut de période
        tableau[0] = tableau[7]; // initialisation du min ac_r
        tableau[1]= tableau[7]; // initialisation du max ac_r
        tableau[2]= myAbsorp.acir; // initialisation du min ac_ir
        tableau[3]= myAbsorp.acir; // initialisation du max ac_ir
        tableau[4] = tableau[4] + 1; // on incrémente le compteur de 1 car on a étudié une valeur de ac
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

                    /* Si le debut commençait de façon croissante et que notre nouvelle valeur est plus petite que notre valeur de départ alors
                     une demi periode est passé*/
                    /* Ou bien si le debut commençait de façon décroissante et que notre nouvelle valeur est plus grande que notre valeur de départ alors
                     une demi periode est passé*/
                    tableau[5]=1;
                }
                min_max(myAbsorp.acr,&tableau[0],&tableau[1]);
                min_max(myAbsorp.acir,&tableau[2],&tableau[3]);
                tableau[4] = tableau[4] + 1;
            }else {
                    if((tableau[6] == 1 && myAbsorp.acr > tableau[7]) || (tableau[6] == 0 && myAbsorp.acr < tableau[7])) {

                        /*on sait que une demi periode est déjà passée
                         Si le debut commençait de façon croissante et que notre nouvelle valeur est plus grande que notre valeur de départ alors
                         une periode est passée en tout */
                        /* Ou bien si le debut commençait de façon décroissante et que notre nouvelle valeur est plus petite que notre valeur de départ alors
                         une periode est passée en tout */

                        ratio = ((tableau[1] - tableau[0]) / myAbsorp.dcr) / ((tableau[3] - tableau[2]) / myAbsorp.dcir);
                        /* Calcul du ratio : Tableau[1]-Tableau[0] : max-min amplitude crête à crête de ac_r
                                             Tableau[3]-Tableau[2] : max-min amplitude crête à crête de ac_ir*/

                        if (ratio <= 1) {   //l'équation entre le ration et spo2 est différente si on est inférieur ou supérieur à 1
                            myOxy.spo2 = (-25) * ratio + 110;   //équations trouvé à l'aide du graphique
                        } else {
                            myOxy.spo2 = (-35.7) * ratio + 121.38;  //équations trouvé à l'aide du graphique

                        }
                        if(tableau[8]==0){
                            tableau[9]=tableau[4];
                            tableau[8]= tableau[4];
                        }
                        myOxy.pouls = 30000 / ((tableau[4]+tableau[8]+tableau[9])/3);   //formule pour calculer la frequence en BPM à partir du nombre de valeur prise pendant une periode
                        tableau[9] = tableau[8];
                        tableau[8] = tableau[4];
                        tableau[4] = 0; // on remet le compteur de valeur à 0 car on a fini une periode

                    }else {
                        min_max(myAbsorp.acr, &tableau[0], &tableau[1]);
                        min_max(myAbsorp.acir, &tableau[2], &tableau[3]);
                        tableau[4] = tableau[4] + 1;
                    }
            }
        }
    }
    return myOxy;
}


void min_max(float value,float* min, float* max){
    if(value < *min){ // mise à jour de min
        *min = value;
    }else {
        if (value > *max) { // mise à jour de max que si value n'est pas déjà un min
            *max = value;
        }
    }
}

void supprime_tableau_mesure(float* tableau){
    free(tableau);
}