// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// A constant for length of the world, here 40 is not suitable because it
// might cause some unexpected bugs in my functions. Limitation of length is controlled in 
// the output step
#define WORD_LENGTH 200	

int sortn = 0; // number of sorting processes

// define a struct to help record each word and its count
typedef struct {
	char word[WORD_LENGTH];
	int count;
} wordrecord;

// main functions for the assignment
void parse(int **pipe); // parse processes
void sort(int **pipein, int **pipeout); // sort processes
void suppress(int **pipe); // suppress processes

// array of pipes used
int **pipe4sort;
int **pipe4supp;

// argument error function
void argError() {
	printf("%s\n", "Wrong format: Please add the number of sorting process...");
	exit(-1);
}

// message error helper function
void msgerror(char *errormessage) {
	printf("Errno = %s\n", errormessage);
	exit(-1);
}

// helper function to convert all the characters in a string to convert case
char *convert(char *str) {
	int i;
	for(i = 0;i < sizeof(str);i++)
		str[i] = tolower(str[i]);
	
	return str;
}

// helper function to find the index of string in an array which has least alphabetic order
int findLeast(int sortn, char **words) {
	int i;
	int index = -1;   // Index of the "least" string.

	for (i = 0; i < sortn; i++) {
		// Skip null pointers
		if (words[i] == NULL)
			continue;

		if (index < 0) {
			index = i;   // Set index to first non-null string
		} else {
			if (strncmp(words[index], words[i], WORD_LENGTH) >= 0) 
				index = i;
			else;
		}
	}
	
	return index;
}


// helper function to help generate pipes
int **create_pipesarray(int sortn) {				
	int **pipes_array = malloc(sizeof(int *) * (sortn));
	int i;
	
	for (i = 0; i < sortn; i++) {
		pipes_array[i] = malloc(sizeof(int) * 2);
	}
	
	return (pipes_array);
}


// sort process
void sort(int **pipein, int **pipeout) {
	pid_t pid;
    int i;
    for (i = 0; i < sortn; i++){
        pipe(pipein[i]);
        pipe(pipeout[i]); 
         
        switch(pid = fork()){
            case -1:
                msgerror("Forking error\n");
                break;
            // child case
            case 0: 
				if (dup2(pipein[i][0], STDIN_FILENO) == -1)
					msgerror("Stdin dup2 fail error...");
				
				close(pipeout[i][0]);
					
				if (dup2(pipeout[i][1], STDOUT_FILENO) == -1)
					msgerror("Stdout dup2 fail error...");
				
				int m;
				int n;
				
                //close the pipein and pipeout pipes
                for (m = 0; m < i+1; m++) {
                	for (n = 0; n < 2; n++) 
                		close(pipein[m][n]);
                }
                for (m = 0; m < i+1; m++) {
                	for (n = 0; n < 2; n++) 
                		close(pipeout[m][n]);
                }

                execlp("sort", "sort", NULL);
                break;
            // parent case
            default:
                close(pipein[i][0]);
        		close(pipeout[i][1]);
                break; 
        }
    }
}


// parse process
void parse(int **pipe) {				
	int i;   
    char buf[WORD_LENGTH];
    FILE *out[sortn];
    
    for (i = 0; i < sortn; i++) {
        out[i] = fdopen(pipe[i][1], "w");
    }
    
    
    //Scan and discard any leading unwanted chars
	int res = scanf("%*[^a-zA-Z]"); 
	
	while (res != EOF){
		res = scanf("%[a-zA-Z]", buf);
		// discard the emptry buffer
		if(buf[0] != '\0') { 
			fputs(convert(buf), out[i % sortn]);
			fputs("\n", out[i % sortn]);
		}
		
		buf[0] = '\0'; //"empty" buffer
		res = scanf("%*[^a-zA-Z]"); 
		i++;
	}
	
    // flush streams
    for (i = 0; i < sortn; i++) {
        fclose(out[i]);
    }
}

// suppress process
void suppress(int **pipe) {
	int i;
	int index;
	char *words[sortn]; // array of words
	FILE *in[sortn]; 
	wordrecord *current = malloc(sizeof(wordrecord));
	char trunc[41];
	
	int empties = 0; //Increments if output from pipe is NULL (meaning EOF)
	pid_t pid = fork();
	// fork process
	switch (pid) {
		
		case -1: 
			break;
		// child case
		case 0:
			// initialize word array
			//char *words[sortn];
			for (i = 0; i < sortn; i++) {
				words[i] = malloc(WORD_LENGTH * sizeof(char));
			}

			for (i = 0; i < sortn; i++) {
				in[i] = fdopen(pipe[i][0], "r");
				if (fgets(words[i], WORD_LENGTH, in[i]) == NULL){
					words[i] = NULL;
					empties++;
				}
			}
			
			// Find the lowest alphabetical word in the array
			index = findLeast(sortn, words);

			// Make this word current word with 1 count
			strncpy(current->word, words[index], WORD_LENGTH);
			current->count = 1;

			while (empties < sortn) {
				if (fgets(words[index], WORD_LENGTH, in[index]) == NULL) {
					words[index] = NULL;
					empties++;
				}
				
				index = findLeast(sortn, words);
				// if the entire array is null, break the loop
				if (index == -1) 
					break;
					
				// If the next word is the same, only increase the count
				if (strncmp(current->word, words[index], WORD_LENGTH) == 0) {
					current->count++;
				// if it's a new word
				} else {
					// check whether the length of the word is greater than 3, if less
					// disgard it, or print out
					if (strlen(current->word) >= 4 && strlen(current->word) <= 41)
						printf("%d---%s", current->count, current->word);
					// to truncate the length of word to 40
					else if (strlen(current->word) >= 40) {
						printf("%s\n", current->word);
						for (i = 0; i <= 39; i++)
							trunc[i] = current->word[i];
						trunc[40] = '\0';
						printf("%d---%s\n", current->count, trunc);
						// here to make it possible to print the results into a file
						fflush(stdout);
					}
					strncpy(current->word, words[index], WORD_LENGTH);
					current->count = 1;
				}
			}

			// for the last word
			if (strlen(current->word) >= 4) {
				printf("%d---%s", current->count, current->word);
				fflush(stdout);
			}
			
			// close the process
			_exit(EXIT_SUCCESS);
			break;
		
		// parent case
		default:
			waitpid(pid, NULL, 0);
			break;
	}
}


// main process
int main(int argc, char **argv) {
	if (argc < 2)
		argError();
	
	// get the number of sorting processes
	sortn = atoi(argv[1]);

	int **pipe4sort = create_pipesarray(sortn); // pipes for sort
	int **pipe4supp = create_pipesarray(sortn); // pipes for suppress

	// sort process
	sort(pipe4sort, pipe4supp);

	// parse process
	parse(pipe4sort);

	// suppress process
	suppress(pipe4supp);
	
	return 0;
}