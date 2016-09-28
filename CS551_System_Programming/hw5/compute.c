// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <rpc/rpc.h>

int current_testing_number =0;
int s;		
unsigned int MAX = 1000;
XDR handleread,handlewrite;
FILE *streamread,*streamwrite;
long estimate_range=0;
long estimate=0;
char *buffer[512];


// sending the message to the manager
void terminate(int signum) {
	printf("Computing get the signal and terminate!\n");
	snprintf(*buffer, sizeof(*buffer), "c|%d", current_testing_number);
	xdr_bytes(&handlewrite, buffer, &MAX, 1000);
	exit(0);
}

// signal handling
void handle_signals() {
	struct sigaction action;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGQUIT);
	action.sa_flags = 0;
	action.sa_mask = set;
	action.sa_handler = terminate;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGHUP, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
}

// compute function
void start_compute(int p, int q) {
	int j = 0;
	
	for(j = p; j <= q; j++) {
		current_testing_number = j;
		int sum = 0, factor;
		for (factor = 1; factor < j - 1; factor++) {
			if (j % factor == 0) {
				sum += factor;
			}
		}
		if (sum == j) {
			snprintf(*buffer, sizeof(*buffer), "p|%d", j);
			xdr_bytes(&handlewrite, buffer, &MAX, 1000);
		}
		estimate++;
	}
	if(estimate_range == 0)
		estimate_range = estimate;
	snprintf(*buffer, sizeof(*buffer), "u|%d|%ld", q, estimate_range);
	
	xdr_bytes(&handlewrite, buffer, &MAX, 1000);
}

// function to process the message from manager
void command_process(char *command) {
	char *info = strtok(command,"|");
	
	switch(*info) {
		case 'r': 
			info = strtok (NULL, "|");
			long start_number = atol(info); 
			info = strtok (NULL, "|");
			long range_size = atol(info);
			printf("Connect successfully, start compute from %ld to %ld\n", start_number, start_number+range_size);
			start_compute(start_number, start_number+range_size);
			break;
		case 's':       // for status
			break;
		case 'k':       //kill by reporter
			printf("kill message received\n");
			snprintf(*buffer, sizeof(*buffer), "c|%d", current_testing_number);
			xdr_bytes(&handlewrite, buffer, &MAX, 1000);
			exit(0);
			break;
		case 'n':
			info = strtok (NULL, "|");
			long old = atol(info); 
			info = strtok (NULL, "|");
			long new_range = atol(info);
			long newend = old + new_range;
			printf("Continue computing with a new range from %ld to %ld\n",old,newend);
			start_compute(old, newend);		
			break;				
	}
}

void time_update(int signum) {
	estimate_range = estimate;
	estimate = 0;
}	


int main (int argc,char **argv) {
	// handle other three signals
	handle_signals();
	
	// estimate the time interval
	struct itimerval itv;
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = time_update;
	sigaction(SIGALRM, &act, NULL);
	memset(&itv, 0, sizeof(itv));
	itv.it_interval.tv_sec = 15;
	itv.it_interval.tv_usec =0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec=0;
	setitimer(ITIMER_REAL, &itv, NULL);	

	// set up the socket connection
	struct sockaddr_in sin; 	
	long address;
	address = *(long *) gethostbyname(argv[1])->h_addr;
	sin.sin_port = atoi(argv[2]);
	sin.sin_addr.s_addr= address;
	sin.sin_family= AF_INET;
	int len;	
	
	// do the while loop
	while(1) { 
		if ((s = socket(AF_INET,SOCK_STREAM,0)) < 0) {
			perror("Socket");
			exit(1);
		}

		// do the connect step
		if (connect (s, (struct sockaddr *) &sin, sizeof (sin)) <0) {
			printf("Cannot find the manager...\n");
			close(s);
			sleep(10);
			continue;
		}
		// connection is sucessful
		printf("This computing process is connected to the manager succesfully, start computing!\n");
		break; 
	}
	
	if ((streamread = fdopen(s,"r")) == (FILE *) -1 ) {
	 	perror("fdopen:");
		exit(1);
	}
	if ((streamwrite = fdopen(s,"w")) == (FILE *) -1 ) {
	 	perror("fdopen:");
		exit(1);
	}

	xdrstdio_create(&handleread,streamread,XDR_DECODE); /* Create XDR handle*/
	xdrstdio_create(&handlewrite,streamwrite,XDR_ENCODE); /* Create XDR handle*/

	long range_size = -1;
	// get the input
	long start_number = atol(argv[3]);
	snprintf(*buffer, sizeof(*buffer), "r|%ld|%ld", start_number, range_size);
	xdr_bytes(&handlewrite, buffer, &MAX, 1000);

	// here deal with the message from manager
	while(1){
		xdr_bytes(&handleread, buffer, &MAX, 1000);
		command_process(*buffer);
	}
}
