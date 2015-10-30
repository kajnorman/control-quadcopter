
#include "gb_common.h"
#include "gb_spi.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "steadysamplerate.c"



void setup_gpio()
{
    INP_GPIO(8);  SET_GPIO_ALT(8,0);
    INP_GPIO(9);  SET_GPIO_ALT(9,0);
    INP_GPIO(10); SET_GPIO_ALT(10,0);
    INP_GPIO(11); SET_GPIO_ALT(11,0);
    INP_GPIO(7); OUT_GPIO(7);//test signal
}

void print_usage() {
    printf("Usage: controlloop -v -c -s num -p Kd -d num -t num\n");
    printf("-v :verbose\n-c csv\n-s setpoint\n-p Proportional gain\n-d Differential gain\n-t added delay in msec\n");
    printf ("These are the connections for the analogue to digital test:\n");
    printf ("jumper connecting GP11 to SCLK\n");
    printf ("jumper connecting GP10 to MOSI\n");
    printf ("jumper connecting GP9 to MISO\n");
    printf ("jumper connecting GP8 to CSnA\n");
    printf ("Potentiometer connections:\n");
    printf ("  (call 1 and 3 the ends of the resistor and 2 the wiper)\n");
    printf ("  connect 3 to 3V3\n");
    printf ("  connect 2 to AD%d\n", 1);
    printf ("  connect 1 to GND\n");
    printf ("connect GP4 to controlsignal for ESC(electronic speed control) for Brushless Motor\n");
}

int main(int argc, char *argv[])
{
    int off,r, v, s, i, pos, sp=0, pv, error, previous_error, integralpart = 0, Y, Kp=0, Kd=0, Ki=0, chan=1/*ad channel 1 on gertboard*/, added_delay = 5, verbose = 0, csv = 0 ;
    int pv_array[32];
    int n,option;
    struct timespec t1,t2;

        //Specifying the expected options
    //The options s,p,d,t expect numbers as argument
    while ((option = getopt(argc, argv,"ovci:s:p:d:t:")) != -1) {
        switch (option) {
             case 'v' : verbose = 1;
                 break;
             case 'c' : csv = 1;
                 break;
             case 'o' : off = 1;
                 break;
             case 's' : sp = atoi(optarg); 
                 break;
             case 'p' : Kp = atoi(optarg);
                 break;
             case 'd' : Kd = atoi(optarg);
                 break;
             case 't' : added_delay = atoi(optarg);
                 break;
             case 'i' : Ki = atoi(optarg);
                 break;
             default: print_usage(); 
                 exit(EXIT_FAILURE);
        }
    }


    
    
    FILE *fp;
    fp = fopen("/dev/servoblaster", "w");
    if ( fp == NULL )
    {
        printf("Error opening file\n");
        return 1;
    }



    
    
    
    // Map the I/O sections
    setup_io();
    
    // activate SPI bus pins
    setup_gpio();
    
    // Setup SPI bus
    setup_spi();
    
    

    if (off == 1){
	 printf("OOOFFFFF\n");
        fprintf(fp, "0=%d\n",50);
	fflush(fp);        
	exit(0);
	}

    printf("SP=%d KP=%d Kd=%d\n",sp,Kp,Kd);

        if (csv){
	    printf("SP;PV;Y;pos\n");
	}



//clock_gettime(CLOCK_REALTIME,&t1);
gettimeofday(&t1,NULL);
printtime(&t1);
added_delay *=  1000;


    for ( ;; )
    {
	do 
//	  clock_gettime(CLOCK_REALTIME,&t2);
	  gettimeofday(&t2,NULL);
	while( diff_time(&t2,&t1)->tv_nsec < added_delay);
	copytime(&t2,&t1);//copy t2 -> t1
        previous_error = error;

        pv = read_adc(chan); //pv >> 5;

        error = sp - pv;
	integralpart = integralpart + (error>>6);
        Y  =  (error * Kp  + (error - previous_error) * Kd + Ki * integralpart);
	Y = Y >> 2;
        if ( Y>1023 )
        {
            Y = 1023;
        }
        if ( Y<0 )
        {
            Y = 0;
        }
        pos = 50 + Y/10;
        fprintf(fp, "0=%d\n",pos);
	fflush(fp);        
        
        if (verbose){
            printtime(&t2);
	    printf("SP=%d,PV=%d,Y=%d,pos=%d        ",sp,pv,Y,pos);
            putchar(0x0D); // go to start of the line
	}
        if (csv){
	    printf("%d;%d;%d;%d\n",sp,pv,Y,pos);
	}
        //      GPIO_SET0 = 0x001;
             GPIO_SET0 = 0x080;
             GPIO_CLR0 = 0x080;
    }
    
    
    fprintf(fp, "0=0\n");
    
    printf("\n");
    restore_io();
    
    
    
    fclose(fp);
    fflush(fp);
    return 0;
}

