#include <stdio.h>
#define setbit(x,y) (x&(1<<y))   

int main()
{
	int x,i,cont=0;
	
	scanf("%d",&x);
	
	for(i=0;i<32;i++){
		if(setbit(x,i))
		{
			cont++;
		}
	}
	
	printf("%d",cont);
	return 0;
}
	
