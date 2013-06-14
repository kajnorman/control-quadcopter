#include "gb_common.h"
#include "gb_spi.h"
#include <unistd.h>


void setup_gpio()
{
    INP_GPIO(8);  SET_GPIO_ALT(8,0);
    INP_GPIO(9);  SET_GPIO_ALT(9,0);
    INP_GPIO(10); SET_GPIO_ALT(10,0);
    INP_GPIO(11); SET_GPIO_ALT(11,0);
}

//
int main(void)
{
    int r, v, s, i, pos, sp, pv, error, previous_error, Y, Kp, Kd, chan;
    int pv_array[32];
    int n,pv_array_index;
    
    
    FILE *fp;
    fp = fopen("/dev/servoblaster", "w");
    if ( fp == NULL )
    {
        printf("Error opening file\n");
        return 1;
    }
    chan = 1; // AD Channel 1  on Gert Board
    
    // Map the I/O sections
    setup_io();
    
    // activate SPI bus pins
    setup_gpio();
    
    // Setup SPI bus
    setup_spi();
    
    
    INP_GPIO(7); OUT_GPIO(7);
    
    
    sp = 400;
    Kp = 1 ;
    Kd = 100 ;//36-uroligt output   ,   12 ingen  virkning
    pv_array_index =0;
        fprintf(fp, "0=100\n");
        fprintf(fp, "0=100\n");
        fprintf(fp, "0=100\n");



    for ( ;; )
    {


	if (GPIO_IN0 &  0x04) 
		GPIO_SET0 = 0x080;
//	else
        GPIO_CLR0 = 0x080;

    }
    // repeated read
    
    
    fprintf(fp, "0=0\n");
    
    printf("\n");
    restore_io();
    
    
    
    fclose(fp);
    fflush(fp);
    return 0;
}

/* EasyCODE ] */
/* EasyCODE ) */

