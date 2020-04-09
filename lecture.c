#include "lecture.h"
#include "fichiers.h"

absorp lecture(FILE* file_pf, int* file_state){
    int octet;
    float valeur=0;
    int i=1, cpt=1;
    absorp myAbsorp;
    while(cpt<5) {
        octet = fgetc(file_pf);
        octet = octet - 48;
        printf("%d\n",octet);
        valeur=additoner(valeur,i,octet);
        printf("%f\n",valeur);
        i++;
        if (i==5){
            octet = fgetc(file_pf);
            modifier(myAbsorp,cpt,valeur);
            cpt ++;
            i=1;
            //printf("%f,%f,%f,%f\n", myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir);
        }
    }
    octet = fgetc(file_pf);
    if(feof(file_pf) == EOF) {
        *file_state = EOF;
    }
    //printf("%f,%f,%f,%f\n", myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir);
	return myAbsorp;

}

float additoner (float nombre, int compteur, int chiffre){
    if(compteur%4==1){
        nombre += 1000*chiffre;
    }
    if(compteur%4==2){
        nombre += 100*chiffre;
    }
    if(compteur%4==3){
        nombre += 10*chiffre;
    }
    if(compteur%4==0){
        nombre += 1*chiffre;
    }
    return nombre;
}

absorp modifier  (absorp my, int compteur, float valeur){
    if(compteur%4==1){

        my.acr=valeur-2047;
    }
    if(compteur%4==2){
        my.dcr=valeur;
    }
    if(compteur%4==3){
        my.acir=valeur-2047;
    }
    if(compteur%4==0){
        my.dcir=valeur;
    }
    return my;
}