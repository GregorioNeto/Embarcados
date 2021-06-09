/*Exercise 10-1: Write a macro that returns TRUE if its parameter is divisible by 10 and FALSE
otherwise. 
* TRADUÇÃO: 
Escreva uma macro que retorne TRUE se seu parâmetro for divisível por 10 e FALSE
de outra forma.*/

#include <stdio.h>
#include <stdlib.h>
#define Division(x) (x%10==0)?1:0


int main()
{
	int number;
	scanf("%d",&number);
	
	if(Division(number))
	{
		printf("TRUE");
	}else printf("FALSE");
	
	return 0;
}
