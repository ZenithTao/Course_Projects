// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
   
void bitmap_init(int size, char *bitmap_array ) {
	bitmap_array = (char *)malloc((size/8 + 1) * sizeof(char));
	memset(bitmap_array, 0, (size/8+1));
}

void bitmap_set(int index, char *bitmap_array) {
	int t = index / 8;
	int remainder = (8- (index+1) % 8)%8;
	char m = (0x1<<remainder);
	bitmap_array[t] |= m;
}

int bitmap_get(int index,  char *bitmap_array) {
	int t = index / 8;
	int remainder =(8- (index+1) % 8)%8;
	char m = (0x1<<remainder);
	char res;
	res = bitmap_array[t] & m;
	if (res > 0)
		return 1;
	else
		return 0;
}