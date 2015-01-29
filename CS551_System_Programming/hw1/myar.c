// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LIANG TAO

#include <stdio.h>
# include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ar.h>
#include <time.h>
#include <utime.h>
#include <stdbool.h>
#include <dirent.h> 
#include <limits.h>

// helper error function
void noFileError(char *file);
void formatError();
void invalidFormatError();
int fchmod(int fd, mode_t mode);
int fchown(int fd, uid_t owner, gid_t group);

// -q, the function to quickly append named files to archive
void quickApp(char *afile, char **files, int numOfFiles) {
	int fromfd; // this is  read file descriptor
	int tofd;   // this is  write file descriptor
	struct ar_hdr ppp; // struct to store the info
	
	// if the archive file is existing, append the info, otherwise create 
	// a new one 
	if ((open(afile, O_RDONLY)) == -1) {
		umask(000);
		tofd = open(afile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
						 | S_IROTH | S_IWOTH);
		write(tofd, ARMAG, SARMAG);
		// put on console the success info
		printf("myar: creating archive archive\n");
	} else 
		tofd = open(afile, O_WRONLY);
		
	// for each file, add the meta info of files into the archive
	for (int i = 0; i < numOfFiles; i++) {
		// check whether the input file is existing
		if (open(files[i], O_RDONLY) == -1)
		 	noFileError(files[i]);
		else {
		 	fromfd = open(files[i], O_RDONLY);
		
			struct stat sb; // to extract the info
			stat(files[i], &sb);
			
			lseek(tofd, 0, SEEK_END);
			// start to read the info into struct
			sprintf(ppp.ar_name, "%s%s", files[i], "/");
			sprintf(ppp.ar_name, "%-16s", ppp.ar_name);
			sprintf(ppp.ar_date, "%-12d", (int)sb.st_mtime);
			sprintf(ppp.ar_uid, "%-6d", (int)sb.st_uid);
			sprintf(ppp.ar_gid, "%-6d", (int)sb.st_gid);
			sprintf(ppp.ar_mode, "%-8o", (int)sb.st_mode);
			sprintf(ppp.ar_size, "%-10d", (int)sb.st_size);
			strncpy(ppp.ar_fmag, ARFMAG, sizeof(ppp.ar_fmag));
			
			write(tofd, &ppp, sizeof(ppp));
			// read the contents of files
			ssize_t nread;
			char buf[100];
			while ((nread = read(fromfd, buf, sizeof(buf))) > 0) 
				write(tofd, buf, nread);
		}
	    close(fromfd);
	}
		close(tofd);
}


// -x extract named files
void extFile(char *afile, char **files, int numOfFiles, int flag) {
	int fromfd, tofd;
	int l;
	struct ar_hdr header;
	struct utimbuf fd_time;
	struct stat sb;
	char buf[100];

	if (open(afile, O_RDONLY) == -1) {
		noFileError(afile);
		exit(-1);
	} else
		fromfd = open(afile, O_RDONLY);
	

	char fileinArc[16]; // each file name in the archive 
	char fileGiven[16]; // file name need to be extract
	
	// skip the SARMAG header info
	lseek(fromfd, SARMAG, SEEK_SET);
	
	for (int i = 0; i < numOfFiles; i++) {
			while((l = read(fromfd, &header, sizeof(header))) == 60) {
				strcpy(fileinArc, strtok(header.ar_name, "/"));
				strcpy(fileGiven, files[i]);
				// if the name is same
				if((strcmp(fileinArc, fileGiven) == 0)) {
					
					tofd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC);
					fchmod(tofd, strtol(header.ar_mode, NULL, 8));
					fchown(tofd, atoi(header.ar_uid), atoi(header.ar_gid));
					stat(files[i], &sb);
					
					// read the file contents
					ssize_t nread;
					for(int i = 0; i < atoi(header.ar_size) / sizeof(buf); i++){
						nread = read(fromfd, buf, sizeof(buf));
						write(tofd, buf, nread);
					}
					nread = read(fromfd, buf, atoi(header.ar_size) % sizeof(buf));
					write(tofd, buf, nread);
					
					// if the flag is -tv, set the time
					if(flag == 0) {
						fd_time.actime = atoi(header.ar_date);
						fd_time.modtime = atoi(header.ar_date);
						utime(files[i], &fd_time);
					}
					// close the write file
					close(tofd);
	
					break;
				}
				// read the next file
				if ((atoi(header.ar_size) % 2) == 0)
					lseek(fromfd, atoi(header.ar_size), SEEK_CUR);
				else
					lseek(fromfd, atoi(header.ar_size) + 1, SEEK_CUR);
			}
		close(fromfd);
	}
}

// print a concise table of files in the archive
void printTable(char *afile, int flag) {
	int fromfd;
	struct ar_hdr ppp;
	// if the archive file doesn't exist, report an error
	if (open(afile, O_RDONLY) == -1) {
		noFileError(afile);
		exit(-1);
	} else
		fromfd = open(afile, O_RDONLY);
	
	// skip the header of the archive file
	lseek(fromfd, SARMAG, SEEK_SET);
	
	int l; // this is the length of the info from each
		   // read process
	while (1) {
		l = read(fromfd, &ppp, sizeof(ppp));
		//printf("p-------\n%s\n", &ppp);
		// while there is no info left, stop reading
		if (l == 0) break;
		// if the format of archive is not correct,
		// report an error
		else if (l != sizeof(ppp)) {
			invalidFormatError();
		}
		
		if (flag) {
			printf("%s\n", strtok(ppp.ar_name, "/"));
			//printf("%s\n", "!!!!!!!!!!");
		} else {
			// print the permissions
			printf( (strtol(ppp.ar_mode, NULL, 8) & S_IRUSR) ? "r" : "-");
			printf( (strtol(ppp.ar_mode, NULL, 8) & S_IWUSR) ? "w" : "-");
			printf( (strtol(ppp.ar_mode, NULL, 8) & S_IXUSR) ? "x" : "-");
	    	printf( (strtol(ppp.ar_mode, NULL, 8) & S_IRGRP) ? "r" : "-");
	    	printf( (strtol(ppp.ar_mode, NULL, 8) & S_IWGRP) ? "w" : "-");
	    	printf( (strtol(ppp.ar_mode, NULL, 8) & S_IXGRP) ? "x" : "-");
	    	printf( (strtol(ppp.ar_mode, NULL, 8) & S_IROTH) ? "r" : "-");
	    	printf( (strtol(ppp.ar_mode, NULL, 8) & S_IWOTH) ? "w" : "-");
	    	printf( (strtol(ppp.ar_mode, NULL, 8) & S_IXOTH) ? "x " : "- ");
	    	
			char time_buf[20];
			time_t time = atoi(ppp.ar_date);
			
			// print the time
			strftime(time_buf, 20, "%b %d %H:%M %Y", localtime(&time));
			// print the other info
			printf("    %d/%d %12ld %s ", atoi(ppp.ar_uid), atoi(ppp.ar_gid),
													atol(ppp.ar_size), time_buf);
			// name print
			printf("%s\n", strtok(ppp.ar_name, "/"));
		}
		// use lseek to skip the contents of the file, notice the odd/even cases
		if ((atoi(ppp.ar_size) % 2) == 0)
			lseek(fromfd, atoi(ppp.ar_size), SEEK_CUR);
		else
			lseek(fromfd, atoi(ppp.ar_size) + 1, SEEK_CUR);
	}
	close(fromfd);
}


// delete named files from archive
void delFile(char *afile, char **files, int numOfFiles){
	int fromfd;
	int tofd;
	int l;
	char buf[100];
	struct ar_hdr header;
		printf("%s\n", "@@@");
	// set a bool array to control the double deletion
	bool flags[numOfFiles];
	for (int i = 0; i < numOfFiles; i++)
		flags[i] = false;
	
	// test whether the archive's name is existing
	// if not, report an error
	if (open(afile, O_RDONLY) == -1) {
		noFileError(afile);
		exit(-1);
	} else
		fromfd = open(afile, O_RDONLY);
	
	// first copy the whole archive file into a new archive file
	char *temp = "temp";
	rename(afile, temp);
	umask(000);
	tofd = open(afile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
						 | S_IROTH | S_IWOTH);
	write(tofd, ARMAG, SARMAG);

	char fileinArc[16]; // each file name in the archive 
	char fileGiven[16]; // file name need to be extract
	
	// skip the SARMAG header info
	lseek(fromfd, SARMAG, SEEK_SET);

	while((l = read(fromfd, &header, sizeof(header))) == 60) { 
		// compare the file name in archive with each given file names
		// need to be deleted
		for (int i = 0; i < numOfFiles; i++) {
			if (!flags[i]) {
				strcpy(fileinArc, strtok(header.ar_name, "/"));
				strcpy(fileGiven, files[i]);
				// compare the names
				// if has the file in the archive
				if((strcmp(fileinArc, fileGiven) != 0)) {
					// write the header info
					write(tofd, &header, sizeof(header));
					
					// read the file contents
					ssize_t nread;
					
					for(int i = 0; i < atoi(header.ar_size) / sizeof(buf); i++){
						nread = read(fromfd, buf, sizeof(buf));
						write(tofd, buf, nread);
					}
	
					nread = read(fromfd, buf, atoi(header.ar_size) % sizeof(buf));
					write(tofd, buf, nread);
					// set the flag
					flags[i] = true;
				}
			}
		}
	}		
		close(fromfd);
		close(tofd);
		
			int m = 0;
		// if the flag is unchanged, report that the file is not in the archive
			for (int i = 0; i < numOfFiles; i++) {
				if (flags[i] == false) {
					noFileError(files[i]); 
					m++;
				}
			}
			// if all the flags are unchanged, remove the new file,
			//rename the temp file and close all the open files
			if (m == numOfFiles) {
				remove(afile);
				rename(temp, afile);
			} else {
				remove(temp);
			}
}


// quickly append all regular files in the current directory, except the archive file
void quickAppAll(char *afile){
	char buf[100]; // this is for reading the file name
	DIR *d;
  	struct dirent *dir;
  	int tofd;
  	char **files;
  	char *name;
  	
  	if (open (afile, O_RDONLY) == -1) {
  		noFileError(afile);
  		exit(-1);
  	} else 
  		tofd = open(afile, O_WRONLY | O_APPEND);
  	
  	// read the files in the current directory
  	d = opendir(".");
  	if (d){
    	while ((dir = readdir(d)) != NULL) {
    		// get the name
      		sprintf(buf, "%s",dir->d_name);
      		struct stat sb; // to extract the info
			lstat(buf, &sb);
			
			// get the mode info
			char mode[8];
			sprintf(mode, "%o", (int)sb.st_mode);
			printf("%s\n", mode);
			if (strcmp(buf, afile) != 0) {
				// if this file is regular, append 
				if(strncmp("100", mode, 3) == 0) {
					files[0] = buf;
					printf("%s\n", buf);
					quickApp(afile, files, 1); // use quickApp() to add the info
				// if this is a symbolic link file
				} else if (strncmp("120", mode, 3) == 0){
					char *ultimateDest;
					// use real path to get the ultimate path
					ultimateDest = realpath(buf, NULL);
					printf("%s\n", ultimateDest);
					write(tofd, ultimateDest, sizeof(ultimateDest));
				}
      		}
    	}
    }
    close(tofd);
    closedir(d);
}

// if named file is not existing, report an error
void noFileError(char *file) {
	printf("%s:  No such file...\n", file);
}

// helper function to report the format error
void formatError() {
	printf("Usage:  myar key archive file ...\n");
	printf("	key:[-q]  quickly append named files to archive\n");
	printf("	key:[-x]  extract named files\n");
	printf("	key:[-xo] extract named files restoring mtime\n");
	printf("	key:[-t]  print a concise table of contents of the archive\n");
	printf("	key:[-tv] print a verbose table of contents of the archive\n");
	printf("	key:[-d]  delete named files from archive\n");
	printf("	key:[-A]  quickly append all 'regular' files in the current directory\n");
	exit(-1);
}

// helper function to report the invalid format erro
void invalidFormatError() {
	printf("Inappropriate archive file...");
	exit(-1);
}

int main(int argc, char *argv[]) {
	if (argc == 0 || argc == 1 || argc == 2 || (strcmp(argv[0], "./myar") != 0)) {
		formatError();
		exit(-1);
	}
	
	// afile is archive files, and the following are files array
	char *afile;
	afile = argv[2];
	
	char *files[1000]; // allocate some memory for files	
	int numOfFiles = argc - 3;
	for (int i = 0; i < argc - 3; i++) {
		files[i] = argv[i+3];
	}
	char *key = argv[1]; // test key, and then use different functions
	if 	    (strcmp("-q", key)  == 0) 	quickApp(afile, files, numOfFiles);
	else if (strcmp("-x", key)  == 0) 	extFile(afile, files, numOfFiles, 1);
	else if (strcmp("-xo", key) == 0) 	extFile(afile, files, numOfFiles, 0);
	else if (strcmp("-t", key)  == 0) 	printTable(afile, 1); 
	else if (strcmp("-tv", key) == 0) 	printTable(afile, 0);
	else if (strcmp("-d", key)  == 0) 	delFile(afile, files, numOfFiles);
	else if (strcmp("-A", key)  == 0) 	quickAppAll(afile);
	else								formatError();
	
}