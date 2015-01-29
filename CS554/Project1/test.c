#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// int read_command() {
// 	char inp[10000];
// 	int i = 0;
// 	char c, d;
// 	while (1) {
// 		if ((c = getchar()) != 'g') {
// 			inp[i++] = c;
// 		} else {
// 			if ((d = getchar()) == 'o') {
// 				return i - 2;
// 			} else {
// 				inp[i++] = c;
// 				inp[i++] = d;
// 				continue;
// 			}
// 		}
// 	}
// }

int read_command(char inp[10000]) {
 	int i = 0;
 	char str[1000];
 	int count = 0;
	while (1) {
		fgets(str, 1000, stdin);
		count++;
		str[strlen(str) - 1] = ' ';
		if (strcmp("go ", str) == 0 || strcmp("gO ", str) == 0 || strcmp("Go ", str) == 0 || strcmp("GO ", str) == 0) {
			inp[i-1] = '\0';
			return i - count + 1;
		} else {
			for (int j = 0; j < strlen(str); j++) {
				inp[i++] = str[j];
			}
		}
	}
}

void uppercase(char *str) {
	for (int i = 0; i < strlen(str); i++) {
		str[i] = toupper(str[i]);
	}
}

char *test(char *current_pos, char *token, int *token_type) {
	int i = 0;
	int j = 0;
	memset(token, 0, sizeof(char));
	while (current_pos[i] == ' ') {
		i++;
	}
	while (current_pos[i] != ' ') {
		token[j++] = current_pos[i++];
	}
	
	token[j] = '\0';
	printf("%s\n", token);
	return current_pos + i;
}

int main(int argc, char const *argv[]) {
	// char inp[10000];
	// char token[100];
	// int m = 100;
	// char *p;
	 
	// int n = read_command(inp);
	// p = test(inp, token, &m);
 //   	p = test(p, token, &m);
 //   	p = test(p, token, &m);
	// p = test(p, token, &m);
	
	char *x = "0hello";
	printf("%d\n", x[0] == 48);
}
