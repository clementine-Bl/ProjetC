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
            FILE* verrou=fopen(".verrouData","r");  //on crée le fichier .verrouData pour que l'interface n'ouvre pas le fichier en meme temps
            fprintf(fichier,"%d\n%d",myOxy.spo2,myOxy.pouls);  //on écrit la valeur de spo2 et de poul dans le fichier Data.txt
            fclose(fichier);  //on ferme le fichier apres avoir ecrit dedans
            remove(".verrouData");  //on supprime le fichier .verrouData quand on a fini d'utiliser le fichier Data.txt
        }
    }
}
