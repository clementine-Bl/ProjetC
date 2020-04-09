#include "lecture.h"
#include "fichiers.h"

absorp lecture(FILE* file_pf, int* file_state){
    int octet, valeur;
    int i=1;
    absorp myAbsorp;
    while(i<22) {
        if (i<5){
            octet = fgetc(file_pf);
            octet = octet - 48;
            valeur =
        }
        if (5<i<10){
            octet = fgetc(file_pf);
            octet = octet - 48;
        }
        if (10<i<15){
            octet = fgetc(file_pf);
            octet = octet - 48;
        }
        if (15<i<20){
            octet = fgetc(file_pf);
            octet = octet - 48;
        }
        i++;
    }
    if(feof(file_pf)) {
        *file_state = EOF;
    }
	return myAbsorp; // return EOF flag

}

