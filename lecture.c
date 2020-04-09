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
        additoner(valeur,i,octet);
        i++;
        if (i==5){
            modifier(myAbsorp,cpt,valeur);
            cpt ++;
        }
    }

    if(feof(file_pf) == EOF) {
        *file_state = EOF;
    }
	return myAbsorp; // return EOF flag

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
        my.acr=valeur;
    }
    if(compteur%4==2){
        my.dcr=valeur;
    }
    if(compteur%4==3){
        my.acir=valeur;
    }
    if(compteur%4==0){
        my.dcir=valeur;
    }
    return my;
}