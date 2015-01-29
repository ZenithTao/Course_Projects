#include <stdio.h>

int main(int argc, char* argv[]) {
	int temp = 0b00000000110000000010000000000011;
	
	int high = temp & (2^10-1);
	int middle = (temp >> 10) & (2^10-1);
	int low = (temp >> 20) & (2^10-1);
	
	printf("%d\n",high );
	printf("%d\n",middle );
	printf("%d\n",low );
}


