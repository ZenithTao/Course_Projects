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


#define KEY (key_t)40008   // KEY for shared memory
#define PERM_FILE		(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define NAMED_FIFO "NAMED_FIFO"

// two structs
typedef struct{
	pid_t compute_pid;
	int total_perfect_result;
	long tested;
	long untested;
} process_struct;

typedef struct{
	pid_t computing_pid;
	long message;
} compute_message;


char fifo_name[50];
int shmid; // shared memory key id
char *bitmap_array; // bit map array
long *result_array; // result array
process_struct *process_array;  // process array
process_struct *current_process;

pid_t *manage_pid;
compute_message message_to_send;
int current_process_index = -1 ;
int start_number=0;
int receiverfd, senderfd = -1, senderfdw = -1;

// helper function to report error
void error_report(char *error_msg) {
	printf("Error: %s", error_msg);
	exit(1);
}

// the functino to send message
void sending_message(compute_message msg) {
	write(receiverfd, &msg, sizeof(msg));
	if(senderfd == -1)
		senderfd = open(fifo_name, O_RDONLY);
	if(senderfdw == -1)
		senderfdw = open(fifo_name, O_WRONLY);
	read(senderfd, &msg, sizeof(msg));
	int num = (int)msg.message;
	
	if (num < 0) {
		current_process_index = -num;
		current_process = process_array - num -1;
	}
}

// the compute process to compute perfect numbers
void computing() {
	long index = 0;
	int p;
	int factor;
	
	while(1) {
		if(bitmap_get(index, bitmap_array) == 0) {
			int sum = 0;
			bitmap_set(index, bitmap_array);
			(*current_process).tested++;
			p = start_number + index;
			
			for (factor = 1; factor < p; factor++) {
				if (p % factor == 0)
					sum += factor;
			}
			// if it is a perfect number, send to the manager
			if (p == sum) {
				(*current_process).total_perfect_result++;
				pid_t pid = getpid();
				message_to_send.computing_pid = pid;
				message_to_send.message = sum;
				sending_message(message_to_send);
			}
		} else {
			(*current_process).untested++;
		}
		index++;
		
		// if the bit index cannot hold the number, stop
		if (p >= 1024000)
			break;
	}
}

// function to handle the sigles and terminal the compute process
void terminate(int signum) {
	printf("Received signals...Terminate the compute process %d...\n", getpid());
	if(current_process_index == -1)
		exit(0);
	
	//Delete the entry of compute process
	int i;
	for(i = current_process_index; i < 19; i++) {
		(*(process_array+i)).compute_pid = (*(process_array+i+1)).compute_pid;
		(*(process_array+i)).total_perfect_result = (*(process_array+i+1)).total_perfect_result;
		(*(process_array+i)).tested = (*(process_array+i+1)).tested;
		(*(process_array+i)).untested = (*(process_array+i+1)).untested;
	}
	
	// set the last
	(*(process_array+19)).compute_pid = 0;
	(*(process_array+19)).total_perfect_result = 0;
	(*(process_array+19)).tested = 0;
	(*(process_array+19)).untested = 0;

	exit(0);
}

// function to handle SIGINT SIGUIT dn SIGHUP signals
void handle_signals(void) {
	sigset_t sigset;
	struct sigaction act;
	sigfillset(&sigset);
	sigprocmask(SIG_SETMASK, &sigset, NULL);
	memset(&act, 0, sizeof(act));
	sigfillset(&act.sa_mask);
	// set the handler function, terminate
	act.sa_handler = terminate;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	
	sigemptyset(&sigset);
	sigprocmask(SIG_SETMASK, &sigset, NULL);
}


// main function
int main(int argc, char **argv) {
	handle_signals();
	
	if (argc != 2 || atoi(argv[1]) < 0)
		error_report("Usage: Invalid arguments...");

	//Set up the memory space pointer
	if((shmid = shmget (KEY, 20 * sizeof(long) + (1024000/8+1)*sizeof(char) + 20 * sizeof(process_struct) + sizeof(pid_t), IPC_CREAT | 0660))==-1)
		error_report("shmget process failed...");
	if((bitmap_array = (shmat(shmid, NULL, 0))) == (void *) -1)
		error_report("shmat process failed...");

	start_number = atoi(argv[1]);
	printf("A new compute process(process id: %d) is starting from %d\n", getpid(), start_number);
	bitmap_init(1024000, bitmap_array);

	result_array = (long *)(bitmap_array+ 1024000/8+1);
	process_array = (process_struct *) (result_array + 20);
	manage_pid = (pid_t *)(process_array+20);
	message_to_send.computing_pid = getpid();
	
	// make a fifo
	snprintf(fifo_name, sizeof(fifo_name), "fifo%ld", (long)message_to_send.computing_pid);
	if(mkfifo(fifo_name, PERM_FILE) == -1 && errno != EEXIST)
		printf("FIFO error...");
	
	receiverfd = open(NAMED_FIFO, O_WRONLY);
	message_to_send.computing_pid = getpid();
	message_to_send.message = 0;
	sending_message(message_to_send);

	//Compute the perfect number
	computing();
	
	// close all the open file descriptors
	close(senderfd);
	close(senderfdw);
	close(receiverfd);
	exit(0);
}
