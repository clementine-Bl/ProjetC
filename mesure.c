#include "mesure.h"
#include "fichiers.h"

oxy mesureTest(char* filename){
    float* tableau = create_tableau_mesure();  // creation du tableau de variables utiles pour la fonction MESURE
	oxy myOxy;
	myOxy.pouls =0; // initialisation
	myOxy.spo2 =0; // initialisation
	absorp myAbsorp;
    FILE* fichier = initFichier(filename);
    int etat =0;
    myAbsorp = lireFichier(fichier,&etat);
    while(etat != EOF) {  //tant qu'on est pas arrivé a la fin du fichier on lit les valeur et on les traite
        myOxy = MESURE(myAbsorp, tableau,myOxy);  //calcul du poul et spo2
        myAbsorp = lireFichier(fichier, &etat);
    }
    finFichier(fichier);
    supprime_tableau_mesure(tableau); //on supprime le tableau, on libere la memoire
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
     Tableau[6] = 0 si on est dans l'attente de la premiere periode et 1 sinon
     Tableau[7] = 0 si on est sur des valeurs positives et  1 si on est sur des valeurs négatives
     Tableau[8] = valeur du poul pendant la periode n-1
     Tableau[9] = valeur du poul pendant la periode n-2
     Tableau[10] = valeur du poul pendant la periode n-3
     Tableau[11] = somme des poul
     Tableau[12] = 1 si la valeur n'est pas la premiere valeur traité et 0 si c'est la premiere valeur traité*/
    float* tableau;
    int i;
    tableau =malloc(13* sizeof(float));  //on alloue 13 espace memoire pour 13 variables
    if (tableau != NULL) {
        for (i=0;i<13;i++){
            tableau[i]=0;  //on initialise chaque variables à 0
        }
    }else{
        printf("le tableau n'a pas pu être crée");
    }
    return tableau;
}

oxy MESURE(absorp myAbsorp, float* tableau,oxy myOxy){
    float ratio;
    int i;
    int compteur_valeur = 1;   //variable qui nous permet de faire la moyenne pour le poul
    if(tableau[6] == 0) {
        if(tableau[12]==0) {  // avec la premiere valeur on va regarder si on commence positif ou negatif pour initialiser tableau[7]
            if (myAbsorp.acr > 0) {
                tableau[7] = 0;
            }else{
                tableau[7] = 1;
            }
            tableau[12]=1;
        }else{
            // on fait rien tant que on est pas passé par 0
            if((tableau[7] ==0 && myAbsorp.acr<=0)||(tableau[7] == 1 && myAbsorp.acr >= 0)) {
                // Si on avait des valeurs positive et que notre nouvelle valeur est negative alors on commence une periode
                // Ou bien si on avait des valeurs negative et que notre nouvelle valeur est positive alors on commence une periode
                tableau[6] = 1;
            }
        }
    }else{
        if(tableau[4]==0){  // on est au debut d'une periode
            // on inverse tableau[7] car on se retrouve avec des valeurs de signes opposé
            if (tableau[7]==1) {
                tableau[7] = 0;
            }else {
                tableau[7] = 1;
            }
            tableau[0] = myAbsorp.acr; // initialisation du min ac_r
            tableau[1]= myAbsorp.acr; // initialisation du max ac_r
            tableau[2]= myAbsorp.acir; // initialisation du min ac_ir
            tableau[3]= myAbsorp.acir; // initialisation du max ac_ir
            tableau[4] = tableau[4] + 1; // on incrémente le compteur de 1 car on a étudié une valeur de ac
            tableau[5]=0;  // on remet à 0 en debut de periode
            tableau[11]=0;   //on remet la somme à 0 en debut de periode
        } else {
            if (tableau[5] != 1) { // On rentre dans ce if tant qu'une demi periode n'est pas faite
                if ((tableau[7] ==0 && myAbsorp.acr<=0)||(tableau[7] == 1 && myAbsorp.acr >= 0)){
                    // Si on avait des valeurs positive et que notre nouvelle valeur est negative alors on a fait une demie periode
                    // Ou bien si on avait des valeurs negative et que notre nouvelle valeur est positive alors on a fait une demie periode
                    tableau[5] = 1;
                    // on inverse tableau[7] car on se retrouve avec des valeurs de signes opposé
                    if (tableau[7]==1) {
                        tableau[7] = 0;
                    }else {
                        tableau[7] = 1;
                    }
                }
                //on met à jour les variables min et max de acr et acir, et on incremente le compteur
                mise_a_jour(&tableau[4], myAbsorp.acr, &tableau[0], &tableau[1], myAbsorp.acir, &tableau[2],&tableau[3]);
            } else {
                if ((tableau[7] ==0 && myAbsorp.acr<=0)||(tableau[7] == 1 && myAbsorp.acr >= 0)) {
                    // Si on avait des valeurs positive et que notre nouvelle valeur est negative alors on a fait une autre demie periode donc une periode en tout
                    // Ou bien si on avait des valeurs negative et que notre nouvelle valeur est positive alors on a fait une autre demie periode donc une periode en tout
                    ratio = ((tableau[1] - tableau[0]) / myAbsorp.dcr) / ((tableau[3] - tableau[2]) / myAbsorp.dcir);
                    /* Calcul du ratio : Tableau[1]-Tableau[0] : max-min amplitude crête à crête de ac_r
                                        Tableau[3]-Tableau[2] : max-min amplitude crête à crête de ac_ir*/
                    if (ratio <=1) {   //l'équation entre le ration et spo2 est différente si on est inférieur ou supérieur à 1
                        myOxy.spo2 = (-25) * ratio + 110;   //équations trouvé à l'aide du graphique
                    } else {
                        myOxy.spo2 = (-35.7) * ratio + 121.38;  //équations trouvé à l'aide du graphique
                    }
                    // on somme les anciennes valeurs de poul
                    for (i = 8; i < 11; i++) {
                        if (tableau[i] != 0) {
                            tableau[11] += tableau[i];
                            compteur_valeur += 1;
                        }
                    }
                    myOxy.pouls = ((30000 / tableau[4]) + tableau[11]) /compteur_valeur; // on ajoute la nouvelle valeur de poul à la sommes et on divise par le nombre de valeurs
                    // on decale les valeurs de poul stocker
                    for (i = 10; i > 8; i--) {
                        tableau[i] = tableau[i - 1];
                    }
                    tableau[8] = 30000 / tableau[4];
                    tableau[4] = 0; // on remet le compteur de valeur à 0 car on a fini une periode
                } else {
                    // si on a pas fini l'autre demie periode, on met à jour les variables min et max de acr et acir, et on incremente le compteur
                    mise_a_jour(&tableau[4], myAbsorp.acr, &tableau[0], &tableau[1], myAbsorp.acir, &tableau[2],&tableau[3]);
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

void mise_a_jour(float* tableau4 ,float value1, float* tableau0 , float* tableau1,float value2, float* tableau2,float* tableau3){
    min_max(value1, tableau0, tableau1);
    min_max(value2, tableau2,tableau3);
    *tableau4 = *tableau4 + 1; // on incremente le compteur de valeur durant une periode
}
void supprime_tableau_mesure(float* tableau){
    free(tableau);
}