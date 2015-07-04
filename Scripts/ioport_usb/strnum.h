#ifndef STRNUM_H_
#define STRNUM_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int stringToInt(const char *cadena);
int isOctNumber(const char *numero);
int isDecNumber(const char *numero);
int isHexNumber(const const char *numero);
int isNumber(const char *numero);
int isBinDigit(const char digito);
int isOctDigit(const char numero);
int isDecDigit(const char numero);
int isHexDigit(const char numero);
int charToInt(const char digito);
int power(int base, int potencia);
int isComand(const char *argumento, const char *comando);
void printByte(const unsigned char valor);

#endif /* STRNUM_H_ */
