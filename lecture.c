#include "lecture.h"
#include "fichiers.h"

absorp lecture(FILE* file_pf, int* file_state) {
    int octet, newoctet;
    float valeur = 0;
    long curseur = -1;
    int i = 1, cpt = 1;
    absorp myAbsorp;


    while (cpt < 5 && *file_state != EOF) {
        octet = fgetc(file_pf);
        octet = octet - 48;
        valeur = additoner(valeur, i, octet);
        i++;
        if (i == 5) {
            octet = fgetc(file_pf);
            if(octet==EOF){
                *file_state =EOF;
            }
            myAbsorp = modifier(myAbsorp, cpt, valeur);
            cpt++;
            i = 1;
            valeur = 0;
        }
    }octet = fgetc(file_pf);
    octet=fgetc(file_pf);
    if(octet ==EOF){
        *file_state =EOF;
    }else {
        fseek(file_pf, curseur, SEEK_CUR);
    }


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
        my.acr=valeur-2048;
    }
    if(compteur%4==2){
        my.dcr=valeur;
    }
    if(compteur%4==3){
        my.acir=valeur-2048;
    }
    if(compteur%4==0){
        my.dcir=valeur;

    }
    return my;
}