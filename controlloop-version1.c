#include "gb_common.h"
#include "gb_spi.h"

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
} // setup_gpio


//
//  Read ADC input 0 and show as horizontal bar
//
int main(void)
{ int r, v, s, i, pos, sp, pv, error, previous_error, Y, Kp, Kd, chan;



 FILE *fp;
 fp = fopen("/dev/servoblaster", "w");
 if (fp == NULL) {
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
 fprintf(fp, "0=0\n");
  (void) getchar();

  printf ("starting ESC . When ready hit enter.\n");
  (void) getchar();
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
  Kd = 0 ;//36-uroligt output   ,   12 ingen  virkning
  for (;;)
  {
    previous_error = error;
    pv = v = read_adc(chan); // 0-1023
    error = sp - pv;
    Y  =  error * Kp  + (error - previous_error) * Kd ;


    if (Y>1023) Y = 1023;
    if (Y<0) Y = 0;



   pos = 100 + Y/10;
   fprintf(fp, "0=%d\n",pos);






    // map to 0-63
    s = v >> 4;
//    printf("SP=%d,PV=%d,Y=%d,pos=%d",sp,pv,Y,pos);
    // show horizontal bar
//    for (i = 0; i < s; i++)
//      putchar('#');
//    for (i = 0; i < 64 - s; i++)
//      putchar(' ');
//    putchar(0x0D); // go to start of the line
//	GPIO_SET0 = 0x001;
	GPIO_SET0 = 0x080;
    short_wait();
	GPIO_CLR0 = 0x080;

// fprintf(fp, "0=150\n");
    
  } // repeated read


 fprintf(fp, "0=0\n");

  printf("\n");
  restore_io();



 fclose(fp);
 fflush(fp); 
 return 0;
}
