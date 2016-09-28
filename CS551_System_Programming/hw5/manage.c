// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include <stdlib.h>   
#include <string.h>  
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <poll.h>
#include <rpc/rpc.h>

#define MAXCLIENTS 6

typedef struct{
		long start_number;
		long end_number;
} range;

range *tested_list = NULL;

unsigned int MAX = 1000;

typedef struct{
	int finished;
	char result_array[100] ;
	int hostname[100];
	long tested;
	range current_range;
} host_info;
XDR handleread,handlewrite;
FILE *streamread,*streamwrite;
struct pollfd pollarray[MAXCLIENTS];
host_info *host_infolist;
char clients[MAXCLIENTS][100];	
int killing_signal = 0;	
int client_number = 0; 
int p = -1;
int COUNT = 0;
char report[10000];
char *reportInfo;

// update function
void update(range newR) {
	int j;
	if(COUNT == 0) {
		COUNT++;
		*tested_list = newR;
	}
	
	for (j = 0; j < COUNT; j++) {
		range newrange = (*(tested_list+j));
		if((newR.start_number >= newrange.start_number && newR.start_number<=newrange.end_number) || 
				(newR.end_number>=newrange.start_number && newR.end_number<=newrange.end_number)) {
			long m = newR.start_number;
			if (newrange.start_number < m)  m = newrange.start_number;
			if (newR.end_number < m)  m = newR.end_number;
			if (newrange.end_number < m)  m = newrange.end_number;
			newrange.start_number = m;
			long n = newR.start_number;
			if (newrange.start_number > n) n = newrange.start_number;
			if (newR.end_number > n) n = newR.end_number;
			if (newrange.end_number > n) n = newrange.end_number;
			newrange.end_number = n;
		} else {
			COUNT++;
			*(tested_list+j) = newR;
		}	
	}
}

// get the new range
range newRange(long s, long size) {
	range result;
	long start = s;
	long end = s + size;
	int j;
	
	for (j = 0; j < COUNT; j++) {
		range newRR = (*(tested_list+j));
		if(start >= newRR.start_number && end <= newRR.end_number){
			start = newRR.end_number;
			end = newRR.end_number;
		}
		else if(start < newRR.start_number)
			end = newRR.start_number;
	}
	result.start_number = start;
	result.end_number = end;
	update(result);
	return result;
}


// signal handler
void handler(int signum) {
	int j;
	for(j = 1 ; j < MAXCLIENTS; j++){
		if((*(host_infolist+j)).finished == 1) {
			if ((streamwrite = fdopen(pollarray[j].fd,"w")) == (FILE *) -1 ) {
	 			perror("fdopen:");
				exit(1);
			}
		xdrstdio_create(&handlewrite,streamwrite,XDR_ENCODE); /* Create XDR handle*/
		char *buffer[1] = {"k"};
		xdr_bytes(&handlewrite, buffer, &MAX, 1000);
		}
		killing_signal = 0;
	}
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
	action.sa_handler = handler;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGHUP, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
}

// get the report info
char *getReportInfo() {
	int j;
	char res1[500];
	char res2[500];
	char res3[500];
	char res4[500];
	for(j = 0; j < MAXCLIENTS; j++) {
		if((*(host_infolist+j)).finished == 1){
			char *res = (*(host_infolist+j)).result_array;
			char *machine_name = &clients[j][0];
			int testedppp = (*(host_infolist+j)).tested;
			int startp = (*(host_infolist+j)).current_range.start_number;
			int endp = (*(host_infolist+j)).current_range.end_number;
			snprintf(res1, sizeof(res1), "************ Report Start ************\nMachine %d (%s) found perfect numbers: %s\n", j, machine_name, res);
			snprintf(res2, sizeof(res2), "It is working on %d to %d\n", startp, endp);
			snprintf(res3, sizeof(res3),"It has tested %d numbers\n************ Report End ************", testedppp);
			strcat(res1, res2);
			strcat(res1, res3);
			strcpy(report, res1);
		}
	}
	return report;
}

// main function
int main (int argc, char **argv) {
	handle_signals();
	
	host_infolist = (host_info *)malloc(MAXCLIENTS * sizeof(host_info));
	tested_list = (range *)malloc(100 * sizeof(range));
	memset(tested_list, 0, 100 * sizeof(range));
	
	struct sockaddr_in sin; /* structure for socket address */
	int s;
	int fd;
	unsigned int len;
	int i,num,count;
	struct hostent *hostentp;
	sin.sin_addr.s_addr= INADDR_ANY;
	sin.sin_port = atoi(argv[1]);

	/* Get an internet socket for stream connections */
	if ((s = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("Socket");
		exit(1);
	}

	/* Do the actual bind */
	if (bind (s, (struct sockaddr *) &sin, sizeof (sin)) <0) {
		perror("bind");
		exit(2);
	}

	// Allow a connection queue for up to 5 clients
	listen(s,5);

	pollarray[0].fd = s;
	pollarray[0].events = POLLIN;
				
	for (i = 1;i <= 5; i++) {pollarray[i].fd=-1;pollarray[i].events=POLLIN;}

	while(1) {
		poll(pollarray,6,-1);   /* no timeout, blocks until some activity*/
		if (pollarray[0].revents & POLLIN) {
			len = sizeof(sin);
			if ((fd= accept (s, (struct sockaddr *) &sin, &len)) <0) {
				perror ("accept");
				exit(3);
			}
			// Find first available slot for new john 
			for (i=1;i<=5;i++) if (pollarray[i].fd == -1) break;
				pollarray[i].fd=fd;
				hostentp=gethostbyaddr((char *)&sin.sin_addr.s_addr,
					sizeof(sin.sin_addr.s_addr),AF_INET);
				strcpy(&clients[i][0], hostentp->h_name);
			}

		// If there are no new connections, process waiting clients
		else for (i = 1;i <= 5; i++) {
			if ((streamread = fdopen(pollarray[i].fd,"r")) == (FILE *) -1 ) {
	 			perror("fdopen:");
				exit(1);
			}
			if ((streamwrite = fdopen(pollarray[i].fd,"w")) == (FILE *) -1 ) {
	 			perror("fdopen:");
				exit(1);
			}

			xdrstdio_create(&handleread,streamread,XDR_DECODE); /* Create XDR handle*/
			xdrstdio_create(&handlewrite,streamwrite,XDR_ENCODE); /* Create XDR handle*/
			
			char *buffer[100];
			
			if ((pollarray[i].fd != -1) && pollarray[i].revents) {
				char buf[100];
				count = xdr_bytes(&handleread, buffer, &MAX, 1000);
				if (count > 0) {
					int j=0;
					char *info = strtok(buf,"|");
					switch (*info) {
						case 'r': 	//register
							info = strtok (NULL, "|");
							long r_start_point = atol(info); 
							info = strtok (NULL, "|");
							long r_range_size = atol(info);
							range newR = newRange(r_start_point, 50000);
							while(newR.start_number == newR.end_number)
								newR = newRange(newR.start_number , 50000);
							snprintf(*buffer, sizeof(*buffer), "r|%ld|%ld", newR.start_number, newR.end_number-newR.start_number);
							xdr_bytes(&handlewrite, buffer, &MAX, 1000);
							(*(host_infolist+i)).tested += newR.end_number-newR.start_number;
							(*(host_infolist+i)).finished = 1;
							(*(host_infolist+i)).current_range = newR;
							client_number++;
							break;
						case 's':	//report
							reportInfo = getReportInfo();
							xdr_bytes(&handlewrite, &reportInfo, &MAX, 1000);
							p = i;
							break;
						case 'k':       //killed by reporter
							for(j = 0 ; j < MAXCLIENTS;j++){
								if((*(host_infolist+j)).finished == 1) {
									if ((streamwrite = fdopen(pollarray[j].fd,"w")) == (FILE *) -1 ) {
	 									perror("fdopen:");
										exit(1);
									}
								xdrstdio_create(&handlewrite,streamwrite,XDR_ENCODE); /* Create XDR handle*/
								xdr_bytes(&handlewrite, buffer, &MAX, 1000);
								}
							}
							killing_signal = 1;
							p = i;
							break;
						case 'p':	//report number
							info = strtok (NULL, "|");
							long perf_num = atol(info);  
  							strcat ((*(host_infolist+i)).result_array," ");
							strcat ((*(host_infolist+i)).result_array, info);
							break;
						case 'n':	// update request.
							info = strtok (NULL, "|");
							long d = atol(info); 
							info = strtok (NULL, "|");
							long sizet = atol(info);
							newR = newRange(d, sizet);
							snprintf(buf, sizeof(buf), "u|%ld|%ld", newR.start_number, newR.end_number - newR.start_number);
							xdr_bytes(&handlewrite, buffer, &MAX, 1000);
							(*(host_infolist+i)).tested += sizet;
							(*(host_infolist+i)).current_range = newR;
							break;
						case 'c':
							info = strtok (NULL, "|");
							printf("%s\n", info);
							long last = atol(info); 
							printf("This compute(%d) has been terminated, the last tested number is: %ld \n", i, last);
							client_number--;
							if(killing_signal == 1 && client_number == 0) {
								reportInfo = getReportInfo();
								if ((streamwrite = fdopen(pollarray[p].fd,"w")) == (FILE *) -1 ) {
	 									perror("fdopen:");
										exit(1);
								}
								xdrstdio_create(&handlewrite,streamwrite,XDR_ENCODE); /* Create XDR handle*/
								xdr_bytes(&handlewrite, &reportInfo, &MAX, 1000);						
								exit(0);
						}
						break;
					}
				}
				
				else {
					close(pollarray[i].fd);
					pollarray[i].fd = -1;
				}
			}
		}
	}
}
