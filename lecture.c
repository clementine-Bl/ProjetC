#include "lecture.h"
#include "fichiers.h"

absorp lecture(FILE* file_pf, int* file_state){
    int octet;
    int i =0;
    while(i<21) {
        octet = fgetc(file_pf);
        printf("%d\n", octet);
        i++;
    }
	absorp myAbsorp;
	*file_state=EOF;
	
	return myAbsorp; // return EOF flag

}

