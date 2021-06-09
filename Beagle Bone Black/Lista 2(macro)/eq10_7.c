#include <stdio.h>
#define RECIPROCAL(number) (1.0 / (number))
int main() {
	float counter; /* Counter for our table */
		for (counter = 1.0; counter < 10.0;counter+= 1.0) 
		{
			printf("1/%f = %f\n", counter, RECIPROCAL(counter));
		}
return (0);
}

//CORREÇÃO:
//linha 2 o define criado nao estava assosiado a nenhum parametro, pois nao estava ligado, havia um espaçço
// => #define RECIPROCAL (number) (1.0 / (number)) => #define RECIPROCAL(number) (1.0 / (number))
