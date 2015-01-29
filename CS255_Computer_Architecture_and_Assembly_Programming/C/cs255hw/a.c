#include <stdio.h>


void print(int x) {
	int test = 0b00100000000000000000000000000000;
	
	for(int i = 1; i <= 30; i++) {
		if((test & x) != 0)
			putchar('1');
		else 
			putchar('0');
	}
}


int int main(int argc, char *argv[])
{
	int m = 13;
	print(m);
}

