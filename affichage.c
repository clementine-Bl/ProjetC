#include "affichage.h"

void affichage(int x,int y){
    if (access(".verrouData",F_OK)!=-1){

    }else{
        FILE* pf=fopen("C:\\Users\PROPRIETAIRE\Desktop\Interface\Data.txt","w+");
        if(pf==NULL){
            printf("dans fichiers.c : erreur ouverture fichier ");
            exit(EXIT_FAILURE);
        }else{
            fprintf(pf,"%d\n%d",x,y);
            fclose(pf);
        }

    }
		
}

