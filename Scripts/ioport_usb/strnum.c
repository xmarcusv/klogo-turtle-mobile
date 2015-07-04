#include"strnum.h"

int stringToInt(const char *cadena)
{
	int longitud = strlen(cadena);

	if(isHexNumber(cadena))
	{
		int i, valor = 0, potencia = 0;

		for(i = longitud - 1; i >= 2; i--)
		{
			valor += power(16, potencia++) * charToInt(toupper(cadena[i]));
		}
		return valor;
	}

	if(isOctNumber(cadena))
	{
		int i, valor = 0, potencia = 0;

		for(i = longitud - 1; i >= 1; i--)
		{
			valor += power(8, potencia++) * charToInt(toupper(cadena[i]));
		}
		return valor;
	}

	if(isBinNumber(cadena))
	{
		int i, valor = 0, potencia = 0;

		for(i = longitud - 1; i >= 2; i--)
		{
			valor += power(2, potencia++) * charToInt(toupper(cadena[i]));
		}
		return valor;
	}

	if(isDecNumber(cadena))
	{
		int i, valor = 0, potencia = 0;

		for(i = longitud - 1; i >= 0; i--)
		{
			valor += power(10, potencia++) * charToInt(toupper(cadena[i]));
		}
		return valor;
	}

	return 0;
}

int isBinNumber(const char *numero)
{
	int longitud = strlen(numero);
	if(longitud >= 3 && numero[0] == '0' && tolower(numero[1]) == 'b')
	{
		int i;
		for(i = 2; i < longitud; i++)
		{
			if(!isBinDigit(numero[i])) return 0;
		}
		return 1;
	}

	return 0;
}

int isOctNumber(const char *numero)
{
	if(numero[0] == '0')
	{
		int i;
		for(i = 1; i < strlen(numero); i++)
		{
			if(!isOctDigit(numero[i])) return 0;
		}
		return 1;
	}

	return 0;
}

int isDecNumber(const char *numero)
{
	int i;
	for(i = 0; i < strlen(numero); i++)
	{
		if(!isDecDigit(numero[i])) return 0;
	}
	return 1;
}

int isHexNumber(const char *numero)
{
	int longitud = strlen(numero);
	if(longitud >= 3 && numero[0] == '0' && tolower(numero[1]) == 'x')
	{
		int i;
		for(i = 2; i < longitud; i++)
		{
			if(!isHexDigit(numero[i])) return 0;
		}
		return 1;
	}

	return 0;
}

int isNumber(const char *numero)
{
	return isHexNumber(numero) || isDecNumber(numero) || isOctNumber(numero) || isBinNumber(numero);
}

int isBinDigit(const char digito)
{
	return digito >= '0' && digito <= '1';
}


int isOctDigit(const char digito)
{
	return digito >= '0' && digito <= '7';
}

int isDecDigit(const char digito)
{
	return digito >= '0' && digito <= '9';
}

int isHexDigit(const char digito)
{
	return (toupper(digito) >= 'A' && toupper(digito) <= 'F') || (digito >= '0' && digito <= '9');
}

int charToInt(const char digito)
{
	if(isDecDigit(digito)) return digito - 48;

	if(digito >= 'A' && digito <= 'F') return digito - 55;

	return 0;
}

int power(int base, int potencia)
{
	int i, resultado = 1;

	if(potencia >= 0)
	{
		for(i = 0; i < potencia; i++)
		{
			resultado *= base;
		}
	}
	else exit(1);

	return resultado;
}

int isComand(const char *argumento, const char *comando)
{
	int i, j;
	int tamArgumento = strlen(argumento);
	int tamComando = strlen(comando);

	if(tamArgumento < tamComando) return 0;

	for(i = tamComando - 1, j = tamArgumento - 1; argumento[j] != '/' && j >= 0; i--, j--)
	{
		if(i < 0) return 0;
		if(comando[i] != argumento[j]) return 0;
	}

	return 1;
}

void printByte(const unsigned char valor)
{
	unsigned char i;
	for(i = 0x80; i > 0; i >>= 1)
	{
		if(valor & i) printf("1");
		else printf("0");
	}
}
