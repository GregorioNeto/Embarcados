#define isSet(num,bit) ( num & ( 1 << bit ))
#include <stdio.h>

int main(){
	int num,i,bits;  //num=0011
	scanf("%x",&num); 
	bits = 0;

	for(i=31;i>=0;i--){
		if(isSet(num,i)!=0){
			bits++; 	//num &
		}
	}
	for(i=31;i>=0;i--){
		if(i > (31-bits)){

			printf("1");
		}else{
			printf("0");
		}
	}

	printf("\n");
	return 0;
}
