// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include "header.h"
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

#define MEMORY_KEY (key_t)40008  // Key for shared memory

typedef struct{
	pid_t compute_pid;
	int total_perfect;
	long tested;
	long untested;
} process_struct;

// helper function to report error
void error_report(char *error_msg) {
	printf("Error: %s", error_msg);
	exit(1);
}

int main(int argc, char **argv) {
	int shmid; // shared memory id 
	char *bitmap_array; // bitmap array
	long *result_array; // result array
	process_struct *process_array;  // precess array
	pid_t *manage_pid;	// help to kill manage process

	if (argc > 2)
		error_report("Invalid argument...");
	
	// create shared memory
	if ((shmid = shmget (MEMORY_KEY, (1024000/8+1)*sizeof(char) + 20 * sizeof(long) + 20 * sizeof(process_struct) + sizeof(pid_t), IPC_CREAT | 0660)) == -1) {
		error_report("Cannot create shared memory...");
	}

	// link to the shared memory
	if ((bitmap_array = (shmat(shmid,0,0))) == (void *)-1) {
		error_report("Cannot link to the shared memory...");
	}

	result_array = (long *)(bitmap_array+ 1024000/8+1);
	process_array = (process_struct *) (result_array + 20);
	manage_pid = (pid_t *)(process_array+20);

	int i;
	//print the out put.
	printf("Perfect numbers have been computed\n");
	for (i = 0; i < 20; i++) {
		if(result_array[i] != 0)
			printf("%ld, ", result_array[i]);
	}
	printf("\n----------------------------------------------------\n");
	
	long totalnum = 0;
	for (i = 0; i<20; i++) {
		if((*(process_array+i)).tested != 0)
			totalnum += (*(process_array+i)).tested;
	}
	printf("Total number of tested: %ld", totalnum);
	printf("\n----------------------------------------------------\n");
	
	printf("Current running processes\n");
	printf("----------------------\n");
	for (i = 0; i < 20; i++) {
		if((*(process_array+i)).compute_pid == 0)
			break;
		printf("compute process id: %d\n", (*(process_array+i)).compute_pid);
		printf("--already tested number: %ld\n", (*(process_array+i)).tested);
		printf("--already skipped number: %ld\n", (*(process_array+i)).untested);
		printf("--already found number: %d\n", (*(process_array+i)).total_perfect);
		printf("-----------\n");
	}
	printf("\n----------------------------------------------------\n");
	
	// if has a '-k' flag, kill the process
	if (argc == 2 && !strcmp(argv[1], "-k")) {
		kill(*manage_pid, SIGINT);
	}
}
