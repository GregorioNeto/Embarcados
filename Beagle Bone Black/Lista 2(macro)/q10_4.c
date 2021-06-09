/*Write a preprocessor macro that swaps two integers. (For the real hacker, write one
that does not use a temporary variable declared outside the macro.)
* TRADUÇÃO: 
Escreva uma macro de pré-processador que troca dois inteiros. (Para o verdadeiro hacker, escreva um
que não usa uma variável temporária declarada fora da macro.)
* */
#include <stdio.h>
#define SWAP(x,y) (y)=(x)+(y);(x)=(y)-(x);(y)=(y)-(x);  //2 = 3; 1=2 ; 3=1

int main()
{
	int numb1,numb2;
	
	scanf(" %d",&numb1);
	scanf(" %d",&numb2);
	
	SWAP(numb1,numb2);
	printf("%d %d",numb1,numb2);
	
	return 0;
}
