#include "lecture.h"

absorp lecture(FILE* file_pf, int* file_state) {
    int octet; // variable qui stoke le caractere lu du fichier
    float valeur = 0; //varaible qui contient la valeur de 4 octets donc de 4 caracteres
    int compteur=0; //variable qui compte le nombre d'octet lu dans une trame
    int i = 0; // nous permet de savoir quelles donnée on traite, si c'est ac_r, dc_r, ac_ir ou dc_r
    absorp myAbsorp;
    int tab_octet[19]; // tableau contenant tous les octects en ascii decimal de la trame : donc contient une trame entiere
    int valid=0; // variable qui passe à 1 quand notre trame est valide
    while(valid!=1){ // On continue tant que notre trame est invalide
        octet = fgetc(file_pf);  //permet de lire un caractere
        test_fin_fichier(&octet,&valid,file_state); // quitte la boucle si nous somme à la fin du fichier

        while (octet!=10 && valid!=1){ //Tant qu'on arrive pas à la fin de notre ligne, fin de ligne = LF = decimal(10)
            tab_octet[compteur]=octet; // On ajoute tout dans notre tableau jusqu'à la dernière valeur avant LF même les virgules
            compteur++;  //on incremente compteur car on a ajouté un octect à la trame
            octet = fgetc(file_pf);
            test_fin_fichier(&octet,&valid,file_state);
        }
        if(compteur==19) { // On verfie que notre tableau est valide donc qu'il possède bien toute nos valeurs, il faut qu'il en ai 19
            valid = 1;
        }else{  //sinon on remet le compteur à 0 et on recommence le while en lisant une nouvelle trame
            compteur = 0;
            octet = fgetc(file_pf); // On retourne à la ligne avec CR
            test_fin_fichier(&octet,&valid,file_state);
        }
    }
    // on va maintenant traité la trame valide récupérée
    while (i < 16 && *file_state != EOF) { // On continue tant qu'on a pas stocker nos 4 valeur ac_r, dc_r, ac-ir et dc_ir
        // on calcule la valeur non centrée à l'aide de 4 octects car on sait que les données sont stockées sur 4 octets consecutifs séparés par une virgule
        valeur = additoner(tab_octet[i], tab_octet[i + 1], tab_octet[i + 2], tab_octet[i + 3]);
        myAbsorp = modifier(myAbsorp, i, valeur); // on met a jour notre valeur dans notre absorp en focntion de i pour savoir quelle donnée on modifie
        i = i + 5; // on avance de 5 : nous permet d'éviter la virgule
    }
    octet = fgetc(file_pf); // Après avoir calculé ac_r dc_r ac_ir et dc_ir, on lis le dernier octet CR et on regarde si on est pas à la fin du fichier
    if(octet ==EOF) {
        *file_state = EOF;
    }
    return myAbsorp;
}


float additoner (int millier, int centaine,int dizaine, int unite){
    float valeur;
    valeur = (millier -48)* 1000 + (centaine-48)*100 + (dizaine-48)* 10 + (unite-48);
    // le -48 sert à récupéré la valeur du charactère en entier : en ascii un char 2 est représenté en decimal par 50 si on fait -48 on retrouve 2
    return valeur;
}

absorp modifier  (absorp my, int compteur, float valeur){
    if(compteur==0){
        my.acr=valeur-2048; // on enlève la valeur moyenne (4095/2 =2047,5) pour centrer sur 0 ac_r
    }
    if(compteur==5){
        my.dcr=valeur;
    }
    if(compteur==10){
        my.acir=valeur-2048; // on enlève la valeur moyenne (4095/2 =2047,5) pour centrer sur 0 ac_r
    }
    if(compteur==15){
        my.dcir=valeur;

    }
    return my;
}

void test_fin_fichier(int* octet,int* valid,int* file_state){
    if(*octet ==EOF) {
        *file_state = EOF;
        *valid=1;
    }
}