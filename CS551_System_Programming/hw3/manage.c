// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#define KEY (key_t)40008  // KEY for shared memory
#define PERM_FILE		(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define NAMED_FIFO "NAMED_FIFO"
	
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

int shmid; // shared memory key id
char *bitmap_array; // bit map array
long *result_array; // result array
process_struct *process_array;  // process array
int total_compute_process = 0;
int total_perfectnum_found = 0;
pid_t *manage_pid;
pid_t current_compute_array[20];
int receiverfd, senderfd = -1, senderfd_w = -1;

// helper function to report error
void error_report(char *error_msg) {
	printf("Error: %s", error_msg);
	exit(1);
}

// function to update the result array
void update(long updatep) {
	// if the total found number is less than 20, than update
	// or discard
	if(total_perfectnum_found < 20) {
		result_array[total_perfectnum_found] = updatep;
		total_perfectnum_found++;
	}
}

// initiate the process entry
void process_entry_init(int total, pid_t new_pid) {
	(*(process_array + total-1)).compute_pid= new_pid;
	(*(process_array + total-1)).total_perfect_result = 0;
	(*(process_array + total-1)).tested = 0;
	(*(process_array + total-1)).untested = 0;
	// update the current compute array
	current_compute_array[total-1] = new_pid;
}


// handler function
void terminate(int signum) {
	printf("Received signals...Terminate the manage process...\n");
	int i;
	//Send signal to all the running computers.
	for(i = 0; i < 20; i++) {
		if((*(process_array + i)).compute_pid == 0)
			break;
		kill(current_compute_array[i], SIGINT);
	}
	// sleep for 5 seconds
	sleep(5);

	// free shared memory
	if (shmdt((char *) bitmap_array) == -1) 
		error_report("failed to do the shmdt process");

	if (shmctl(shmid,IPC_RMID,0) == -1)
		error_report("failed to do the shmctl process");
	
	exit(0);
}

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
int main(int argc, char ** argv) {
	handle_signals();
	
	//Set up the memory space pointer
	if((shmid = shmget (KEY, 20 * sizeof(long) + (1024000/8+1)*sizeof(char) + 20 * sizeof(process_struct) + sizeof(pid_t), IPC_CREAT | 0660))==-1)
		error_report("shmget process failed...");

	if((bitmap_array = (shmat(shmid, NULL, 0))) == (void *) -1)
		error_report("shmat process failed...");

	bitmap_init(1024000, bitmap_array);
	result_array = (long *)(bitmap_array+ 1024000/8+1);
	process_array = (process_struct *) (result_array + 20);
	manage_pid = (pid_t *)(process_array+20);
	*manage_pid = getpid();

	//Register the compute process.
	int receiverfd, senderfd, receiverfdw = -1;
	compute_message msg;
	char fifo_name[50];
	printf("A manage process is starting (process id: %d)...\n", *manage_pid);
	
	// make a fifo
	if(mkfifo(NAMED_FIFO, PERM_FILE) == -1 && errno != EEXIST)
		printf("FIFO error...");
	
	receiverfd = open(NAMED_FIFO, O_RDONLY);
	receiverfdw = open(NAMED_FIFO, O_WRONLY);
	
	while(1) {
		read(receiverfd, &msg, sizeof(msg));
		//Registe the compute process.	
		if(msg.message == 0) {
			total_compute_process++;
			msg.message = -total_compute_process;
			printf("A computing process registers sucessfully... PID : %d\n", msg.computing_pid);
			process_entry_init(total_compute_process, msg.computing_pid);
		}
		//Update the perfect number arrays
		else if(msg.message>0) {
			printf("A perfect number is found and submited...\n");
			update(msg.message);
		}
		snprintf(fifo_name, sizeof(fifo_name), "fifo%ld", (long)msg.computing_pid);
		senderfd = open(fifo_name, O_WRONLY);
		write(senderfd, &msg, sizeof(msg));
		close(senderfd);
	}
	close(receiverfd);
	close(receiverfdw);
	exit(0);
}
