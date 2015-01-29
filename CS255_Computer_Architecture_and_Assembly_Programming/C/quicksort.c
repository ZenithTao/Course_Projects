#include <stdio.h>

void quicksort(int s[], int l, int r) {
	
	if(l < r) {
		int i = l; int j = r; int x = s[l];
		
	while (i < j) {
		while(i < j && s[j] >= x)
			j--;
		if(i < j)
			s[i++] = s[j];
		
		while(i < j && s[i] < x)
			i++;
		if(i < j)
			s[j--] = s[i];
	}
	s[i] = x;
	quicksort(s, l, i - 1);
	quicksort(s, i + 1, r);
}
	
}

int main(int argc, char *argv[]) {
	int s[] = {1343,343,1434,1,43,1,4,34234,134,134,345};
	quicksort(s, 0, 10);
	for(int i = 0; i < 10; i++)
		printf("s[%d] = %d\n", i, s[i]);
}