#include <stdio.h>
#include <stdlib.h>
#define DIE \
fprintf(stderr, "Fatal Error:Abort\n");exit(8);

int main() {

	int value;
	value = 1;
	if (value < 0)
	{
		DIE;
	}
	printf("We did not die\n");
return (0);
}
//* CORREÇÃO:
	//é necessário colocar chaves do if no Die, pois ele vai indicar se tem erro
	//* no caso nao ta entrando no if por causa das chaves e esta indo para o DIE e o die n ta idicando erro, apenas saindo
