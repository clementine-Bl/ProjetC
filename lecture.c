#include "lecture.h"
#include "fichiers.h"

absorp lecture(FILE* file_pf, int* file_state) {
    int octet;
    float valeur = 0;
    int cpt=0;
    int i = 0;
    absorp myAbsorp;
    int tab_octet[19]; // tableau contenant les valeur en ascii decimal de mes valeurs ac_r dc_r ac_ir et dc_r
    int valid=0; // variable qui passe à 1 quand notre tram est valide
    while(valid!=1){ // On continue tant que notre tram (tab) est invalide
        octet = fgetc(file_pf);
        if(octet ==EOF) { //verification de fin de fichier
            *file_state = EOF;
            valid=1; // quitte la boucle si nous somme à la fin du fichier
        }
        while (octet!=10 && valid!=1){ //Tant qu'on arrive pas à la fin de notre ligne LF : hexa (0A) : decimal(10)
            tab_octet[cpt]=octet; // On ajoute tout dans notre tableau jusqu'à la dernière valeur avant LF même les virgules
            cpt++;
            octet = fgetc(file_pf);
            if(octet ==EOF) {
                *file_state = EOF;
                valid=1;
            }

        }
        if(cpt==19) { // On verfie que notre tableau est valide, s'il possède bien toute nos valeurs
            valid = 1;
        }else{
            cpt = 0;
            octet = fgetc(file_pf); // On retourne à la ligne avec CR et on recommence une nouvelle tram
            if(octet ==EOF) {
                *file_state = EOF;
                valid=1;
            }
        }
    }
    while (i < 16 && *file_state != EOF) { // On continue tant qu'on a pas fait nos 4 valeur ac_r dc_r ac-ir et dc_ir
        valeur = additoner(tab_octet[i], tab_octet[i + 1], tab_octet[i + 2], tab_octet[i + 3]); // on calcule la valeur non centrée
        myAbsorp = modifier(myAbsorp, i, valeur); // on met a jour notre valeur dans notre absorp
        i = i + 5; // on avance de 5 : nous permet d'éviter la virgule
    }
    octet = fgetc(file_pf); // Après avoir calculé ac_r dc_r ac_ir et dc_ir on revient à la ligne et on regarde si on est pas à la fin du fichier
    if(octet ==EOF) {
        *file_state = EOF;
    }
	return myAbsorp;
}


float additoner (int millier, int centaine,int dizaine, int unite){
    float valeur;
    valeur = (millier -48)* 1000 + (centaine-48)*100 + (dizaine-48)* 10 + (unite-48);
    // le -48 sert à récupéré charactère en décimal : en ascii un char 2 est représenté en decimal par 50 si on fait -48 on retrouve 2
    return valeur;
}

absorp modifier  (absorp my, int compteur, float valeur){
    if(compteur==0){
        my.acr=valeur-2048; // on enlève la valeur moyenne pour centrer sur 0 ac_r
    }
    if(compteur==5){
        my.dcr=valeur;
    }
    if(compteur==10){
        my.acir=valeur-2048; // on enlève la valeur moyenne pour centrer sur 0 ac_r
    }
    if(compteur==15){
        my.dcir=valeur;

    }
    return my;
}

