#include "fir.h"
#include "fichiers.h"

absorp firTest(char* filename){
    float** parametre_FIR=create_tableau_FIR();
    int cpt=0;
	absorp	myAbsorp;
	absorp newAbsorp;
	FILE* record1 = initFichier(filename);
	int etat =0;
	newAbsorp = lireFichier(record1,&etat);
	while(etat != EOF){
        myAbsorp = FIR(newAbsorp,&cpt,parametre_FIR);
        newAbsorp = lireFichier(record1,&etat);
	}
	finFichier(record1);
	supprime_tableau_FIR(parametre_FIR);

	return myAbsorp;

}

float** create_tableau_FIR(){    //tableau a 2 lignes : premiere ligne contient les ACR  et  la deuxieme contient les ACIR
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
    absorp newAbsorp;
    float sommeAC_R = FIR_TAPS[0]*valueAbsorp.acr;
    float sommeAC_IR = FIR_TAPS[0]*valueAbsorp.acir;
    int hcpt;
    int i;
    if (*cpt>50) {
        hcpt = 50;
    }else {
        hcpt = *cpt;
    }
    for (i=0;i<hcpt;i++){
        sommeAC_R += FIR_TAPS[i+1] * (tableau[0][(*cpt-i) % 50]);
        sommeAC_IR += FIR_TAPS[i+1] * (tableau[1][(*cpt-i) % 50]);
    }
    *cpt = *cpt + 1;
    tableau[0][*cpt % 50] = valueAbsorp.acr;
    tableau[1][*cpt % 50] = valueAbsorp.acir;
    newAbsorp.acr= sommeAC_R;
    newAbsorp.acir =sommeAC_IR;
    newAbsorp.dcir =valueAbsorp.dcir;
    newAbsorp.dcr = valueAbsorp.dcr;
    return newAbsorp;
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