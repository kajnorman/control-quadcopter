#include "gb_common.h"
#include "gb_spi.h"
#include <unistd.h>

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
}

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
    printf ("When ready hit enter.\n");
    fprintf(fp, "\n");
    printf ("starting ESC . When ready hit enter.\n");
    fprintf(fp, "0=100\n");
    printf ("starting Controll loop . When ready hit enter.\n");
    (void) getchar();
    
    
    // Map the I/O sections
    setup_io();
    
    // activate SPI bus pins
    setup_gpio();
    
    // Setup SPI bus
    setup_spi();
    
    // The value returned by the A to D can jump around quite a bit, so
    // simply printing out the value isn't very useful. The bar graph
    // is better because this hides the noise in the signal.
    
    
    INP_GPIO(7); OUT_GPIO(7);
    
    
    sp = 400;
    Kp = 1 ;
    Kd = 100 ;//36-uroligt output   ,   12 ingen  virkning
    pv_array_index =0;
    /* EasyCODE - */
    printf("enter value for SP : ");
    scanf("%d",&sp);
    /* EasyCODE - */
    printf("enter value for KP : ");
    scanf("%d",&Kp);
    /* EasyCODE - */
    printf("enter value for Kd : ");
    scanf("%d",&Kd);

    printf("SP=%d KP=%d Kd=%d\n",sp,Kp,Kd);

    for ( ;; )
    {
        previous_error = error;

        //read adc into pv_array
        for ( n=0;n<32;n++ )
        {
            pv_array[n] = read_adc(chan);
        }

	//calculating moving average pv        
        pv = 0;
        for ( n=0;n<32;n++ )
        {
            pv += pv_array[n]>>5;
        }

        error = sp - pv;
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
        
        
        
        
        
        
            printf("SP=%d,PV=%d,Y=%d,pos=%d",sp,pv,Y,pos);
            putchar(0x0D); // go to start of the line
        //      GPIO_SET0 = 0x001;
             GPIO_SET0 = 0x080;
         short_wait();
             GPIO_CLR0 = 0x080;
        usleep(1000);  //usleep(100)   3kHz
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

/* EasyCODE ] */
/* EasyCODE ) */

