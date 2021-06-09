/*Write a second macro is_hex that returns true if its argument is a hex digit (0-9, A-F,
a-f). The second macro should reference the first.
* TRADUÇÃO: Escreva uma segunda macro is_hex que retorne true se seu argumento for um dígito hexadecimal (0-9, A-F,
a-f). A segunda macro deve fazer referência ao primeiro.
* */


#include <stdio.h>
#define if_hex(x) ((((x>='0')&&(x<='9')) || ((x>='a')&&(x<='f')) || ((x>='A')&&(x<='F')))?1:0)

int main()
{
	char number;
	scanf("%c",&number);
	if(if_hex(number)==1)
	{
		printf("True");
	}else
		printf("False");
	
	
	return 0;
}
