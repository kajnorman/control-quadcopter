#include "gb_common.h"


void setup_gpio()
{
   INP_GPIO(7);  OUT_GPIO(7);
} // setup_gpio


//
//  Read ADC input 0 and show as horizontal bar
//
int main(void)
{ 
  setup_io();
  setup_gpio();

  while (1)
  {
//   short_wait();
	GPIO_CLR0 = 0x080;
//   short_wait();
	GPIO_SET0 = 0x080;
  }

  return 0;
}
