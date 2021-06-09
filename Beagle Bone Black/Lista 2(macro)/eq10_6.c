#include <stdio.h>
#define SQR(x) ((x) * (x))

int main(){
	int counter; 
	counter = 0;
	while (counter < 5)
	{
		printf("x %d square %d\n", counter, SQR(counter));
		counter++;
	}
return (0);
}
// CORREÇÃO:
//nesse algoritmo tivemos que tirar o cunter++ do SQR(counter++)
//pois ele estava dando resultados inerente e o contador nao estava rodando.
//ele estava calculando o cont e subistiduindo.
//colocamos o contador em baixo para poder icrementar certo.

//* * ALGORITIMO ERRADO *
//#include <stdio.h>
//#define SQR(x) ((x) * (x))
//int main(){
//int counter; /* counter for loop */
//Counter = 0;
//while (counter < 5)
//printf("x %d square %d\n", counter, SQR(++counter));
//return (0);
//}
/*
