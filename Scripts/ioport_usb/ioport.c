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
#include <dirent.h>

#define SET_DATA_DIR_IN		0x20
#define SET_DATA_DIR_OUT	0x0F

/* LOCAL VARIABLES DECLARATIONS */

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
	else if(isComand(argv[0], "inbusb")) return doInbUsb(argc,argv); 
	else if(isComand(argv[0], "vinbusb")) return doVinbUsb(argc,argv);
	else if(isComand(argv[0], "outbusb")) return doOutbUsb(argc,argv);
	else if(isComand(argv[0], "xoutbusb"))return doXoutbUsb(argc,argv);

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
	printf("metodo não implementado");
	return 1;
}

int doVinbUsb(int argc, char *argv[])
{
	printf("metodo não implementado");
	return 1;
}

int doOutbUsb(int argc, char *argv[])
{
	if(argc == 3 && isNumber(argv[2]))
	{
		
		int valor, devfd, ret;
		char* puerto;

		puerto = argv[1];
		valor = stringToInt(argv[2]);

		printf("outb %s: ", puerto);

		printValue(valor);

		printf("\n");

		devfd = open(puerto, O_RDWR);
		if (devfd == -1) {
			printf("Não foi possivel acessar %s, verifique se o endereço está correto e se possui permissão.\n", puerto);
			return 0;
		}

		ret = ioctl(devfd, PPCLAIM);
		if (ret < 0) {
			printf("\nret = %d, errno = %d, Ocorreu um erro ao reinvidicar acesso a porta!!\n\r", ret, errno);
			close(devfd);
			return 1;
		}

		int	data_direction = 0; //Output mode
		ret = ioctl(devfd, PPDATADIR, &data_direction);		

		ret = ioctl(devfd, PPWDATA, &valor);
		if (ret < 0) {
			printf("\n---ret = %d, errno = %d, Erro ao acessar o registro da porta2!!\n\r", ret, errno);
			return 1;
		}

		ret =  ioctl(devfd, PPRELEASE);
		close(devfd);
		printf("close %d \n",ret);

		return 0;
	}else if(argc == 2 && isNumber(argv[1])){
		DIR *d;
		struct dirent *dir;
		d = opendir("/dev");
		if(d)
		{ 	
			while((dir = readdir(d)) != NULL)
			{
				if(strstr(dir->d_name,"parport") != NULL){		
					int valor, devfd, ret;			
					char* puerto;

					puerto = malloc(strlen(dir->d_name)+1+5);
					strcpy(puerto,"/dev/");
					strcat(puerto,dir->d_name);					
					
					valor = stringToInt(argv[1]);

					printf("outb %s: ", puerto);

					printValue(valor);

					printf("\n");

					devfd = open(puerto, O_RDWR);
					if (devfd == -1) {
						printf("Não foi possivel acessar %s, verifique se o endereço está correto e se possui permissão.\n", puerto);
						continue;
					}

					ret = ioctl(devfd, PPCLAIM);
					if (ret < 0) {
						printf("\nret = %d, errno = %d, Ocorreu um erro ao reinvidicar acesso a porta!!\n\r", ret, errno);
						close(devfd);
						continue;
					}

					int	data_direction = 0; //Output mode
					ret = ioctl(devfd, PPDATADIR, &data_direction);		

					ret = ioctl(devfd, PPWDATA, &valor);
					if (ret < 0) {
						printf("\n---ret = %d, errno = %d, Erro ao acessar o registro da porta2!!\n\r", ret, errno);
						continue;
					}

					ret =  ioctl(devfd, PPRELEASE);
					close(devfd);
					printf("close %d \n",ret);

					return 0;
				}				
			}
			printf("Nenhum dispositivo encontrador, utilizar: outbusb <numero da porta> <valor>\n");
			return 1;
		}
	}

	printf("modo de uso: outbusb <numero da porta> <valor>\n");
	return 1;
}

int doXoutbUsb(int argc, char *argv[])
{
	printf("metodo não implementado");
	return 1;
}
