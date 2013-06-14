#include <stdio.h>
#include <stdlib.h>

int main(void)
{
 FILE *fp;

 fp = fopen("/dev/servoblaster", "w");

 if (fp == NULL) {
	 printf("Error opening file\n");
	 exit(0); 
	 }

 fprintf(fp, "0=150\n");

 fclose(fp);
 fflush(fp); 
 return 0;
}
