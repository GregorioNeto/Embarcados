#include <stdio.h>
#define SQR(x) (x * x)
int main(){
	
	int counter;
	for (counter = 0; counter < 5; ++counter) 
	{
		printf("x %d, x squared %d\n",counter+1, SQR(counter+1));
	}
return (0);
}
 //* CORREÇÃO:
 //* está ocorrendo um erro de prioridade
 //*  count= 0 , x= 1 , sqr = 0+1*1+0 = 1 => 2
 //	  count= 1 , x= 2 , sqr = 1+1*1+1 = 3 => 4
 //   count= 2 , x= 3 , sqr = 2+1*1+2 = 5 => 9
 //	  count= 0 , x= 1 , sqr = 3+1*1+3 = 7 => 16
 //	  count= 0 , x= 1 , sqr = 4+1*1+4 = 9 => 25
 
 //era para somar antes de multiplicar, entao ficaria (counter+1)*(counter+1)
 //apenas necessita colocar os parenteses no SQR((counter+1)));
