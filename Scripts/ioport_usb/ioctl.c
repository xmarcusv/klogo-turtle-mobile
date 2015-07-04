/* INCLUDE FILE DECLARATIONS */
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/sockios.h>
#include <linux/types.h>
#include <linux/ppdev.h>
#include <pthread.h>
#include <errno.h>
#include <sys/io.h>
#include "strnum.h"

#define SET_DATA_DIR_IN		0x20
#define SET_DATA_DIR_OUT	0x0F

/* LOCAL VARIABLES DECLARATIONS */

inline unsigned long STR_TO_U8(const char *cp)
{
	int result = 0,value;
	int offset = 4;
	unsigned int base;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result + (value << offset);
		cp++;
		offset = offset - 4;
	}

	return result;
}

int doInb(int argc, char *argv[]);
int doVinb(int argc, char *argv[]);
int doOutb(int argc, char *argv[]);
int doXoutb(int argc, char *argv[]);
void printValue(int valor);
//implementação adicionada para dar suporte a adaptador usb
int doInbUsb(int argc, char *argv[]);
int doVinbUsb(int argc, char *argv[]);
int doOutbUsb(int argc, char *argv[]);
int doXoutbUsb(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	if(isComand(argv[0], "inb")) return doInb(argc,argv);
	else if(isComand(argv[0], "vinb")) return doVinb(argc,argv);
	else if(isComand(argv[0], "outb")) return doOutb(argc,argv);
	else if(isComand(argv[0], "xoutb"))return doXoutb(argc,argv);

	//implementação adicionada para dar suporte a adaptador usb
	else if(isComand(argv[0], "inbusb")) return doInb(argc,argv);
	else if(isComand(argv[0], "vinbusb")) return doVinb(argc,argv);
	else if(isComand(argv[0], "outbusb")) return doOutb(argc,argv);
	else if(isComand(argv[0], "xoutbusb"))return doXoutb(argc,argv);

	return 0;
}

int doInb(int argc, char *argv[])
{
	if(argc == 2 && isNumber(argv[1]))
	{
		int puerto, valor;

		puerto = stringToInt(argv[1]);
	
		ioperm(puerto, 1, 1);

		valor = inb(puerto);

		ioperm(puerto, 1, 0);

		printf("0x%X\n", valor);

		return valor;
	}

	printf("modo de uso: inb <numero da porta>\n");

	exit(1);
}


int doVinb(int argc, char *argv[])
{
	if(argc == 2 && isNumber(argv[1]))
	{
		int puerto, valor;

		puerto = stringToInt(argv[1]);
	
		ioperm(puerto, 1, 1);

		valor = inb(puerto);

		ioperm(puerto, 1, 0);

		printf("vinb 0x%X: ", puerto);

		printValue(valor);

		printf("\n");

		return valor;
	}

	printf("modo de uso: vinb <numero da porta>\n");

	exit(1);
}

int doOutb(int argc, char *argv[])
{
	if(argc == 3 && isNumber(argv[1]) && isNumber(argv[2]))
	{
		int puerto, valor;
		puerto = stringToInt(argv[1]);
		valor = stringToInt(argv[2]);

		printf("outb 0x%X: ", puerto);

		printValue(valor);

		printf("\n");
	
		ioperm(puerto, 1, 1);

		outb(valor, puerto);

		ioperm(puerto, 1, 0);

		return 0;
	}

	printf("modo de uso: outb <numero da porta> <valor>\n");

	return 1;
}

int doXoutb(int argc, char *argv[])
{
	if(argc == 3 && isNumber(argv[1]) && isNumber(argv[2]))
	{
		int puerto, valor, lectura;
		puerto = stringToInt(argv[1]);
		valor = stringToInt(argv[2]);

		printf("xoutb 0x%X: ", puerto);

		printValue(valor);

		printf("\n");

		ioperm(puerto, 1, 1);

		lectura = inb(puerto);
		valor ^= lectura;
		outb(valor, puerto);

		ioperm(puerto, 1, 0);

		return 0;
	}

	printf("modo de uso: xoutb <numero da porta> <valor>\n");

	return 1;
}

void printValue(int valor)
{
	printf("( 0b");

	printByte(valor);

	fprintf(stdout, " | 0x%X | %d )", valor, valor);
}

//implementação adicionada para dar suporte a adaptador usb

int doInbUsb(int argc, char *argv[])
{
	return 1;
}

int doVinbUsb(int argc, char *argv[])
{
	return 1;
}

int doOutbUsb(int argc, char *argv[])
{
	if(argc == 3 && isNumber(argv[1]) && isNumber(argv[2]))
	{
		int puerto, valor, devfd;
		unsigned char	write_value;
		puerto = stringToInt(argv[1]);
		valor = stringToInt(argv[2]);

		printf("outb 0x%X: ", puerto);

		printValue(valor);

		printf("\n");

		devfd = open(puerto, O_RDWR);

		write_value = STR_TO_U8(input_value);
		ioctl(devfd, PPWDATA, &write_value);

		ioctl(devfd, PPRELEASE);

		return 0;
	}
	
	printf("modo de uso: outb <numero da porta> <valor>\n");
	return 1;
}

int doXoutbUsb(int argc, char *argv[])
{
	return 1;
}
