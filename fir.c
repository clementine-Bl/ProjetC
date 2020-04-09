#include "fir.h"
#include "fichiers.h"

absorp firTest(char* filename){
    float** parametre_FIR=create_tableau_FIR(); // creation de notre tableau pour FIR
    int cpt=0; // initialisation de notre nombres de valeurs total à 0
	absorp	myAbsorp;
	absorp newAbsorp; // creation d'un newAbsorp pour eviter de renvoyer que des 0 quand notre etat passe à EOF
	FILE* record1 = initFichier(filename);
	int etat =0;
	newAbsorp = lireFichier(record1,&etat);
	while(etat != EOF){ // continue tant qu'on est pas rendu à la fin de notre fichier
        myAbsorp = FIR(newAbsorp,&cpt,parametre_FIR);
        newAbsorp = lireFichier(record1,&etat);
	}
	finFichier(record1);
	supprime_tableau_FIR(parametre_FIR); // libère l'espace pris par notre tableau
	return myAbsorp;
}

float** create_tableau_FIR(){
    /*Detail Tableau :
      Tableau[0] : ligne qui représente les ancienne valeur de ac_r (50)
      Tableau[1] : ligne qui représente les ancienne valeur de ac_ir (50)
      Fin détail tableau */
    float** tableau;
    tableau = malloc(2*sizeof(float*));
    if(tableau == NULL ){
        printf("Le tableau n'a pas pu être créé");
    }else{
        tableau[0]=malloc(50* sizeof(float));
        tableau[1]=malloc(50* sizeof(float));
    }
    return tableau;
}

absorp FIR(absorp valueAbsorp,int *cpt, float** tableau){
    //cpt représente le nombre totale de variable ac_r ou ac_ir que nous avons
    float sommeAC_R = FIR_TAPS[0]*valueAbsorp.acr; // x_ac_r[n] * h(0)  : début de notre somme
    float sommeAC_IR = FIR_TAPS[0]*valueAbsorp.acir; // x_ac_ir[n] * h(0)
    int hcpt; //valeur qui indique le nombres de valeurs présentent dans notre tableau
    int i;
    if (*cpt>50) { // mise à jour de hcpt : si cpt > 50 notre tableau sera forcement rempli donc hcpt=50
        hcpt = 50;
    }else {
        hcpt = *cpt;
    }
    for (i=0;i<hcpt;i++){
        sommeAC_R += FIR_TAPS[i+1] * (tableau[0][(*cpt-i) % 50]); // calcul de la somme  h(i+1)*x[n-i]
        sommeAC_IR += FIR_TAPS[i+1] * (tableau[1][(*cpt-i) % 50]);
    }
    /*le modulo sert à savoir ou se trouve nos valeur exemple : de x(0) à x(49) ils seront rentrés dans l'ordre dans le tableau
     mais pour x(50) nous allons le rentrer à la place de x(0) et x(51) à la place de x(1)
     si nous somme à n=51 alors notre somme est x(51)*h(1) + x(50)*h(2) ... (51%50)=1 donc tableau[0][1] * h(1)*/

    *cpt = *cpt + 1; // on incrémente notre nombres de variable total
    tableau[0][*cpt % 50] = valueAbsorp.acr; // mise à jour du tableau ac-r avec la nouvelle valeur myAbsorp.acr
    tableau[1][*cpt % 50] = valueAbsorp.acir; // mise à jour du tableau ac-ir avec la nouvelle valeur myAbsorp.acir
    valueAbsorp.acr= sommeAC_R; // mise à jour de notre valeur ac_r filtré
    valueAbsorp.acir =sommeAC_IR; // mise à jour de notre valeur ac_ir filtré
    return valueAbsorp;
}

float FIR_TAPS[51]={
        1.4774946e-019,
        1.6465231e-004,
        3.8503956e-004,
        7.0848037e-004,
        1.1840522e-003,
        1.8598621e-003,
        2.7802151e-003,
        3.9828263e-003,
        5.4962252e-003,
        7.3374938e-003,
        9.5104679e-003,
        1.2004510e-002,
        1.4793934e-002,
        1.7838135e-002,
        2.1082435e-002,
        2.4459630e-002,
        2.7892178e-002,
        3.1294938e-002,
        3.4578348e-002,
        3.7651889e-002,
        4.0427695e-002,
        4.2824111e-002,
        4.4769071e-002,
        4.6203098e-002,
        4.7081811e-002,
        4.7377805e-002,
        4.7081811e-002,
        4.6203098e-002,
        4.4769071e-002,
        4.2824111e-002,
        4.0427695e-002,
        3.7651889e-002,
        3.4578348e-002,
        3.1294938e-002,
        2.7892178e-002,
        2.4459630e-002,
        2.1082435e-002,
        1.7838135e-002,
        1.4793934e-002,
        1.2004510e-002,
        9.5104679e-003,
        7.3374938e-003,
        5.4962252e-003,
        3.9828263e-003,
        2.7802151e-003,
        1.8598621e-003,
        1.1840522e-003,
        7.0848037e-004,
        3.8503956e-004,
        1.6465231e-004,
        1.4774946e-019
};

void supprime_tableau_FIR(float** tableau){
    free(tableau[0]);
    free(tableau[1]);
    free(tableau);
}