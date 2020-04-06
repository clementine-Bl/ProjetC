#include "affichage.h"



void affichage(int x,int y){
    if (access(".verrouData",F_OK)!=-1){
        printf("Attention l'interface lit les données");
    }else{
        FILE* pf=fopen("Data.txt","w+");
        if(pf==NULL){
            printf("dans fichiers.c : erreur ouverture fichier ");
            exit(EXIT_FAILURE);
        }else{

            fprintf(pf,"%d\n%d",x,y);
            remove(".verrouData");
        }

    }
		
}

