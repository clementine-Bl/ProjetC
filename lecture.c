#include "lecture.h"
#include "fichiers.h"

absorp lecture(FILE* file_pf, int* file_state){
    int octet;
    int i=0;
    while(i<21) {
        if (i<4){
            octet = fgetc(file_pf);
            octet = octet - 48;
        }
        octet = fgetc(file_pf);

        printf("%d\n", octet);
        i++;
    }
	absorp myAbsorp;
    if(feof(file_pf)) {
        *file_state = EOF;
    }
	return myAbsorp; // return EOF flag

}

