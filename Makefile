controllop: controlloop.c gb_common.o gb_spi.o
	gcc controlloop.c -o controlloop gb_common.o gb_spi.o
