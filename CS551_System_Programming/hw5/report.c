// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include <stdio.h>
#include <sys/types.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <rpc/rpc.h>

int s;	// socket file descriptor	
char *buffer[512];
unsigned int MAX = 1000;
XDR handleread,handlewrite;
FILE *streamread,*streamwrite;

int main (int argc,char *argv[]) {
	long address;
	struct sockaddr_in sin; // socket address for destination
	// manager address
	address = *(long *) gethostbyname(argv[1])->h_addr;
	sin.sin_addr.s_addr= address;
	sin.sin_family= AF_INET;
	sin.sin_port = atoi(argv[2]);

	while(1) {
		// keeping loop
		if ((s = socket(AF_INET,SOCK_STREAM,0)) < 0) {
			perror("Socket");
			exit(1);
		}

		// connect to the manager
		if (connect (s, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
			printf("No manager found...\n");
			close(s);
			sleep(10);
			continue;
		}
		
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

	// if invoke with a "-k" flag, kill the manager
	if(argc == 4 && !strcmp(argv[3], "-k")) {
		printf("Kill the manager.\n");
		char *buffer[1] = {"k"};
		xdr_bytes(&handlewrite, buffer, &MAX, 1000);
	} else {
		char *buffer[1] = {"s"};
		xdr_bytes(&handlewrite, buffer, &MAX, 1000);
	}
	
	while ((xdr_bytes(&handleread, buffer, &MAX, 1000) > 0)) {
		printf("%s\n", *buffer);
	}
	exit(0);
}
