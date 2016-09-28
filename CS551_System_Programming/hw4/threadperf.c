// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <string.h>  
#include <time.h>

// a thread struct to store information
typedef struct {
	pthread_t thread_id;
	int thread_index; 
	int perfect_num_res[50]; // found perfect number array
	long total_thread_tested; // number that this thread has tested
	long total_thread_skipped; // number that this thread has skipped
	int isIdle ; // Idle status
	int block_index; 
	int perfect_number_current_thread_found; // number of perfect number that current thread found 
} thread_struct;

// thread array
thread_struct *thread_array; 
// gloabla variables from input
long MAX, BLOCK;
// global variables help to get the result when quit
int total_tested = 0;
int total_thread_number = 0;
// make array to control the thread
pthread_mutex_t mutex[100];
pthread_cond_t cond[100];
// time variables
clock_t cpu_time_start, cpu_time_end;
double cpu_time_used;
time_t process_time_start;

// bitmap
char *bitmap = NULL;
// a mutex for operation on bitmap  
pthread_mutex_t bitmap_mtx= PTHREAD_MUTEX_INITIALIZER; 
   
// functions on bitmap
void bitmap_init(int size) {
	bitmap = (char *)malloc((size/8 + 1) * sizeof(char));
	memset(bitmap, 0, (size/8 + 1));
}

void bitmap_set(int index) {
	int q = index / 8;
	int remainder = (8- (index+1) % 8) % 8;
	char x = (0x1<<remainder);
	pthread_mutex_lock(&bitmap_mtx);
	bitmap[q]|= x;
	pthread_mutex_unlock(&bitmap_mtx);
}

int bitmap_get(int index) {
	int q = index / 8;
	int remainder = (8- (index+1) % 8) % 8;
	char x = (0x1<<remainder);
	char res;
	pthread_mutex_lock(&bitmap_mtx);
	res = bitmap[q] & x;
	pthread_mutex_unlock(&bitmap_mtx);
	return res > 0 ? 1:0;
}


// helper function to get the numbers tested
int all_total_tested() {
	int sum = 0;
	int i;
	for(i = 0; i < total_thread_number; i++) {
		sum += (*(thread_array + i)).total_thread_tested;
	}
	return sum;
}

// quit function
void quit() {
	printf("####### Quit the computation and all the threads #######\n");
	//Report summary
	printf("Pefect number found: ");
	int i;
	for(i = 0; i < total_thread_number; i++) {
		int j;
		for(j = 0; j < (*(thread_array+i)).perfect_number_current_thread_found; j++){
			printf("%d ", (*(thread_array+i)).perfect_num_res[j]);
		}
	}
	printf("\n");

	printf("Total number tested: %d \n", all_total_tested());
	
	// CPU time
	cpu_time_end = clock();
    cpu_time_used = ((double) (cpu_time_end - cpu_time_start)) / CLOCKS_PER_SEC;
	printf("CPU time in seconds: %f \n", cpu_time_used);
	// process elapsed time
	float process_time_elapsed = difftime(time(NULL), process_time_start);
	printf("Elapsed time in seconds: %f\n", process_time_elapsed);
	
	printf("---------------------------------------------------------------\n");
	exit(0);
}

// idle function
void idleThread(int thread_index) {
	// array is 0-based
	thread_index--;
	pthread_mutex_lock(&mutex[thread_index]);
	(*(thread_array + thread_index)).isIdle = 1;
	pthread_mutex_unlock(&mutex[thread_index]);
}

// restart function
void restartThread(int thread_index) {
	// array is 0-based
	thread_index--;
	pthread_mutex_lock(&mutex[thread_index]);
	(*(thread_array + thread_index)).isIdle = 0;
	pthread_cond_signal(&cond[thread_index]);
	pthread_mutex_unlock(&mutex[thread_index]);
}

// wait function
void wait_S(int time_seconds) {
	printf("---------------------------------------------------------------\n");
	printf("Wait %d seconds before processing additional commands...\n", time_seconds);
	sleep(time_seconds);
	printf("---------------------------------------------------------------\n");
}

// compute function
void *compute(void *pstruct) {
	thread_struct *current_struct = (thread_struct *)pstruct;
    int index = (*current_struct).block_index;
	int thread_index = (*current_struct).thread_index;
	
	while(1) {
		// check the idle status
		pthread_mutex_lock(&mutex[thread_index]);
		if ((*(thread_array + thread_index)).isIdle) {
			pthread_cond_wait(&cond[thread_index], &mutex[thread_index]);
		}
		pthread_mutex_unlock(&mutex[thread_index]);

		if (bitmap_get(index) == 0) {
			bitmap_set(index);
		
			int start_num = (index - 1) * BLOCK + 1;
			
			(*(thread_array + thread_index)).block_index = index;
			
			int i;
			for (i = 0; i < BLOCK; i++) {
				// check whether the number is perfect
				int check = start_num + i;
				int sum = 0;
				int factor;
				for(factor = 1; factor <=(check -1); factor++) {
					if((check % factor) ==0) {
						sum += factor;
					}
				}
				if(sum == check) {
					(*(thread_array + thread_index)).perfect_num_res[(*(thread_array+thread_index)).perfect_number_current_thread_found] = start_num + i;
					(*(thread_array+thread_index)).perfect_number_current_thread_found++;
				}
				//update total tested number
				(*(thread_array+thread_index)).total_thread_tested++;
				
				// wrap around and continue
				if(check == MAX) {
					index = 0;
					break;
				}
			}
			
			index++;
			(*(thread_array + thread_index)).block_index = index;
		}
		
		else {
			(*(thread_array + thread_index)).total_thread_skipped += BLOCK;
			index++;
		}
		
		// if the number tested is greater than the MAX, quit and exit
		if(all_total_tested() >= MAX) {
			quit();
		}
		
	}
}

// start K function
void startK(int K) {
	// test the input
	if (K <= 0 || K >= MAX) {
		printf("%s\n", "invalid input of K, please choose another...");
		return;
	}
	
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	
	int current_block = (int)((double)K/BLOCK) + 1;
	(*(thread_array+total_thread_number)).thread_index = total_thread_number;
	(*(thread_array+total_thread_number)).block_index = current_block;

	// take advantage of multiple processors
	pthread_mutex_init(&mutex[total_thread_number], NULL);
	pthread_cond_init(&cond[total_thread_number], NULL);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	
	if(pthread_create(&tid, &attr, compute, (void *)(thread_array + total_thread_number)) == 0){
		(*(thread_array+total_thread_number)).thread_id = tid;		
		total_thread_number++;
		printf("A new thread has been started, thread number: %d\n", total_thread_number);
	}
}


// report function
void report() {
	printf("########################## Report #########################\n");
	
	int i;
	for(i = 0; i < total_thread_number ; i++) {
		thread_struct c = (*(thread_array+i));
		printf("Thread %d (tid = %d) has tested %ld numbers, skipped %ld numbers.\nIt's now working on block %d. ", c.thread_index+1, (int)c.thread_id, c.total_thread_tested, c.total_thread_skipped, c.block_index);
		if(c.isIdle) {
			printf("(Status: Idle)\n\n");
		} else {
			printf("(Status: Running)\n\n");
		}
		if (c.perfect_number_current_thread_found > 0) {
			// print out the perfect numbers this thread found
			printf("Perfect numbers found by thread %d\n", c.thread_index + 1);
			printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");	
			int j;
			for(j = 0; j < c.perfect_number_current_thread_found; j++) {
				printf("%d ", c.perfect_num_res[j]);
			}
			printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
		}
	}
	printf("###########################################################\n");
}


// main function
int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: Invalid argument number...\n");
		printf("Please add MAX and BLOCK...\n");
		exit(1);
	}
	
	cpu_time_start = clock();
	process_time_start = time(NULL);
	
	bitmap_init(1024000);

	thread_array = (thread_struct *)malloc(40 * sizeof(thread_struct));
	
	MAX = atol(argv[1]);
	BLOCK = atol(argv[2]);
	if (MAX <= 0 || BLOCK <= 0) {
		printf("%s\n", "Invalid MAX or BLOCK, please choose another one...\n");
		exit(0);
	}
	
	sigset_t mask;
    struct sigaction action;
	// deal with the signals
	sigemptyset (&mask);
    sigaddset (&mask, SIGINT);
    sigaddset (&mask, SIGHUP);
    sigaddset (&mask, SIGQUIT);
    action.sa_flags = 0;
    action.sa_mask = mask;
    action.sa_handler = quit;
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGHUP, &action, NULL);
    sigaction (SIGQUIT, &action, NULL);
	
	int n; // help to deal with the command
	sigjmp_buf env;
	char command[50];
	if(sigsetjmp(env, 0)){
		fgets(command, 50, stdin);
		// get the name of command
		// startK process
		if(strncmp(command,"start", 5) == 0) {
			n = atoi(command+6);
			startK(n);
		}
		// idle the specific thread
		else if(strncmp(command, "idle", 4) == 0) {
			int n = atoi(command + 5);
			idleThread(n);
		}
		//restart the specific thread
		else if(strncmp(command, "restart", 7) == 0) {
			int n = atoi(command + 8);
			restartThread(n);
		}
		// wait the commands
		else if(strncmp(command, "wait", 4) == 0) {
			int n = atoi(command + 5);
			wait_S(n);
		}
		// report process
		else if(strncmp(command, "report", 6) == 0) {
			report();
		}
		// quit process
		else if(strncmp(command, "quit", 4) == 0) {
			quit();
		}
		// or if it's an invalid argument...
		else {
			printf("%s\n", "Usage: Invalid argument...");
		}
		printf("***************************************************************\n");
	}
	
	siglongjmp(env,1);
	return 0;
}
