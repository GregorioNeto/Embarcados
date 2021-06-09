#define split(num) (num & 0xF) //0xF = 1111 & 0011
#define deslc(num) (num >> 4) //num >> 4 0011 4x
#include <stdio.h>

int main(){
  int num;
  int b0,b1,b2,b3;

  scanf("%i",&num);

  b0 = split(num);
  num = deslc(num);
  b1 = split(num);
  num = deslc(num);
  b2 = split(num);
  num = deslc(num);
  b3 = split(num);

  printf("%d | %d | %d | %d \n", b3, b2, b1, b0);
  return 0;
}
