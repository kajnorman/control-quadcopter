#include "gb_common.h"
#include "gb_spi.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


// Set GPIO pins to the right mode
// DEMO GPIO mapping:
//         Function            Mode
// GPIO0=  unused
// GPIO1=  unused
// GPIO4=  pwm controll signal for Brushless motord
// GPIO7=  unused
// GPIO8=  SPI chip select A   Alt. 0
// GPIO9=  SPI MISO            Alt. 0
// GPIO10= SPI MOSI            Alt. 0
// GPIO11= SPI CLK             Alt. 0
// GPIO14= unused
// GPIO15= unused
// GPIO17= unused
// GPIO18= unused
// GPIO21= unused
// GPIO22= unused
// GPIO23= unused
// GPIO24= unused
// GPIO25= unused
//

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
}

int main(int argc, char *argv[])
{
    int r, v, s, i, pos, sp=0, pv, error, previous_error, Y, Kp=0, Kd=0, chan=1/*ad channel 1 on gertboard*/, added_delay = 0, verbose = 0, csv = 0 ;
    int pv_array[32];
    int n,option;

        //Specifying the expected options
    //The options s,p,d,t expect numbers as argument
    while ((option = getopt(argc, argv,"vcs:p:d:t:")) != -1) {
        switch (option) {
             case 'v' : verbose = 1;
                 break;
             case 'c' : csv = 1;
                 break;
             case 's' : sp = atoi(optarg); 
                 break;
             case 'p' : Kp = atoi(optarg);
                 break;
             case 'd' : Kd = atoi(optarg);
                 break;
             case 't' : added_delay = atoi(optarg);
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
    
    printf ("These are the connections for the analogue to digital test:\n");
    printf ("jumper connecting GP11 to SCLK\n");
    printf ("jumper connecting GP10 to MOSI\n");
    printf ("jumper connecting GP9 to MISO\n");
    printf ("jumper connecting GP8 to CSnA\n");
    printf ("Potentiometer connections:\n");
    printf ("  (call 1 and 3 the ends of the resistor and 2 the wiper)\n");
    printf ("  connect 3 to 3V3\n");
    printf ("  connect 2 to AD%d\n", chan);
    printf ("  connect 1 to GND\n");
    printf ("connect GP4 to controlsignal for ESC(electronic speed control) for Brushless Motor\n");
    
    
    // Map the I/O sections
    setup_io();
    
    // activate SPI bus pins
    setup_gpio();
    
    // Setup SPI bus
    setup_spi();
    
    
    printf("SP=%d KP=%d Kd=%d\n",sp,Kp,Kd);

        if (csv){
	    printf("SP;PV;Y;pos\n");
	}



    for ( ;; )
    {
        previous_error = error;

        //read adc into pv_array
//        for ( n=0;n<32;n++ )
//        {
//            pv_array[n] = read_adc(chan);
//        }

	//calculating moving average pv        
  //      pv = 0;
    //    for ( n=0;n<32;n++ )
      //  {
 //           pv += pv_array[n];
   //     }
        pv = read_adc(chan); //pv >> 5;

        error = sp - pv;
	error = error>>4;
        Y  =  error * Kp  + (error - previous_error) * Kd ;
        if ( Y>1023 )
        {
            Y = 1023;
        }
        if ( Y<0 )
        {
            Y = 0;
        }
        pos = 100 + Y/10;
        fprintf(fp, "0=%d\n",pos);
        
        
        if (verbose){
	    printf("SP=%d,PV=%d,Y=%d,pos=%d        ",sp,pv,Y,pos);
            putchar(0x0D); // go to start of the line
	}
        if (csv){
	    printf("%d;%d;%d;%d\n",sp,pv,Y,pos);
	}
        //      GPIO_SET0 = 0x001;
             GPIO_SET0 = 0x080;
         short_wait();
             GPIO_CLR0 = 0x080;
        usleep(added_delay);  //usleep(100)   3kHz
        // fprintf(fp, "0=150\n");
    }
    // repeated read
    
    
    fprintf(fp, "0=0\n");
    
    printf("\n");
    restore_io();
    
    
    
    fclose(fp);
    fflush(fp);
    return 0;
}

