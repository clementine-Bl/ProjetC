#include "fir.h"
#include "fichiers.h"
float* createtab(){
    float* tableau;
    tableau = malloc(50*sizeof(float));
    if(tableau == NULL ){
        printf("Le tableau n'a pas pu être créé");
    }
    return tableau;

}
absorp firTest(char* filename){
    float* tab_Ac_Cap_R = createtab();
    float* tab_Ac_Cap_IR = createtab();
    int cpt=0;
	absorp	myAbsorp;
	absorp newAbsorp;
	FILE* record1 = initFichier(filename);
	int etat =0;
	newAbsorp = lireFichier(record1,&etat);
	while(etat != EOF){
        myAbsorp = fir(newAbsorp,&cpt,tab_Ac_Cap_R,tab_Ac_Cap_IR);
        newAbsorp = lireFichier(record1,&etat);
	}
	finFichier(record1);

	return myAbsorp;

}


absorp fir(absorp valueAbsorp,int *cpt, float* tab_Ac_Cap_R, float* tab_Ac_Cap_IR){
    absorp newAbsorp;
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

    float sommeAC_R = FIR_TAPS[0]*valueAbsorp.acr;
    float sommeAC_IR = FIR_TAPS[0]*valueAbsorp.acir;
    int hcpt;
    if (*cpt>50) {
        hcpt = 50;
    }else {
        hcpt = *cpt;
    }

    for (int i =0; i<hcpt;i++){
        sommeAC_R += FIR_TAPS[i+1] * (tab_Ac_Cap_R[(*cpt-i) % 50]);
        sommeAC_IR += FIR_TAPS[i+1] * (tab_Ac_Cap_IR[(*cpt-i) % 50]);
    }
    *cpt = *cpt + 1;
    tab_Ac_Cap_R[*cpt % 50] = valueAbsorp.acr;
    tab_Ac_Cap_IR[*cpt % 50] = valueAbsorp.acir;


    newAbsorp.acr= sommeAC_R;
    newAbsorp.acir =sommeAC_IR;
    newAbsorp.dcir =valueAbsorp.dcir;
    newAbsorp.dcr = valueAbsorp.dcr;

    return newAbsorp;
}

