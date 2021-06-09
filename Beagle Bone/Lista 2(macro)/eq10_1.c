#include <stdio.h>
#define FIRST_PART 7
#define LAST_PART 5
#define ALL_PARTS FIRST_PART + LAST_PART // CORREÇÃO ALL_PARTS (FIRST_PART + LAST_PART)

int main() 
{ 
	printf("The square of all the parts is %d\n", ALL_PARTS * ALL_PARTS);
	return (0);
}

/* EXPLICAÇÃO:
NA LINHA 7 OCORRE:			 7 + 5 * 7 + 5 OU FIRST_PART + LAST_PART * FIRST_PART + LAST_PART 
ENTÃO VAI DA PRIORIDADE A MULTIPLICAÇÃO E A RESPOSTA FICA = 47
CORRIGIDO 				(7 + 5) * (7 + 5)
*/ 
