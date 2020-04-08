#include "affichage.h"


void affichage(oxy myOxy){
    if (access(".verrouData",F_OK)!=-1){
        // on regarde si le fichier .verrouData existe,
        // s'il existe c'est que l'interface lis les données donc on ne peut pas effecteur deux choses sur un meme fichier en meme temps
        printf("Attention l'interface lit les données\n");
    }else{
        // comme il n'existe pas on peut modifier le fichier Data.txt
        FILE* fichier=fopen("Data.txt","w+");  //on ouvre le fichier en w+ : en ecriture avec effacement du contenu
        if(fichier==NULL){
            printf("erreur ouverture fichier");  // on affiche une erreur si le fichier n'est pas trouvé
            exit(EXIT_FAILURE);
        }else{
            FILE* verrou=fopen(".verrouData","r");
            fprintf(fichier,"%d\n%d",myOxy.spo2,myOxy.pouls);
            remove(".verrouData");
            fclose(fichier);
        }
    }
}
