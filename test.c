#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    
    
    FILE *fp;
    fp = fopen("/dev/servoblaster", "w");
    if ( fp == NULL )
    {
        printf("Error opening file\n");
        return 1;
    }


        fprintf(fp, "0=70\n");
	fflush(fp);
	sleep(1);
        fprintf(fp, "0=50\n");
	fflush(fp);

    
    fclose(fp);
    fflush(fp);
    return 0;
}

