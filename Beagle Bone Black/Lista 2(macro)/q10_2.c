/*Write a macro is_digit that returns TRUE if its argument is a decimal digit.
 * TRADUÇÃO: Escreva uma macro is_digit que retorne TRUE se estes argumentos é um digito decimal
*/
#include <stdio.h>
#define if_digit(x) (((x>='0')&&(x<='9'))?1:0)

int main()
{
	char number;
	scanf("%c",&number);
	if(if_digit(number))
	{
		printf("True");
	}else
		printf("False");
	
	
	return 0;
}
