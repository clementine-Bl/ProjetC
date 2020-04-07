#include "affichage.h"


void affichage(oxy myOxy){
    if (access(".verrouData",F_OK)!=-1){
        printf("Attention l'interface lit les données\n");
    }else{
        FILE* fichier=fopen("Data.txt","w+");
        if(fichier==NULL){
            printf("erreur ouverture fichier");
            exit(EXIT_FAILURE);
        }else{
            FILE* verrou=fopen(".verrouData","w");
            fprintf(fichier,"%d\n%d",myOxy.spo2,myOxy.pouls);
            remove(".verrouData");
            fclose(fichier);
        }
    }
}
