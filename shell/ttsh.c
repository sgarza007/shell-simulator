/*
 * The Tiny Torero Shell (TTSH)
 *
 * Add your top-level comments here.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>

#include "parse_args.h"

#include "history_queue.h"


// TODO: add your function prototypes here as necessary
void queueEntry(char *cmdline, int *count);
void executeFunc(int bg, char *argv[]);
void myHandler(int sig);

int main( ){ 

	char cmdline[MAXLINE];
	char *argv[MAXARGS];
	int bg;
	int count = 0;
	//	int chld_pid;
//	signal(SIGCHLD, myHandler);
	// TODO: add a call to signal to register your signal handler on 
	//       SIGCHLD here
	//signal(SIGCHLD, xyz);
	while(1) {
		// (1) print the shell prompt
		fprintf(stdout, "ttsh> ");  
		fflush(stdout);

		// (2) read in the next command entered by the user
		//	perror("fgets error");
		fgets(cmdline, MAXLINE,stdin);
		//truncates the new line at the end of cmdline so we can compare it
		//using strcmp
		cmdline[strcspn(cmdline,"\n")] = '\0';
		queueEntry(cmdline, &count);
		if (feof(stdin)) { /* End of file (ctrl-d) */
			fflush(stdout);
			exit(0);
		}

		fprintf(stdout, "DEBUG: %s\n", cmdline);

		// TODO: complete top-level steps
		// (3) make a call to parseArguments function to parse it into its argv
		// format
		bg = parseArguments(cmdline, argv);
		// (4) determine how to execute it, and then execute it

		executeFunc(bg, argv);
	}
	return 0;
}

void executeFunc(int bg, char *argv[])
{
	if (strcmp(argv[0], "exit") == 0)
	{
		exit(0);
	}
	if (strcmp(argv[0], "history")==0)
	{
		print_queue();
	}
	if(bg==0){
		int chld_pid;
		chld_pid = fork();
		if (chld_pid == 0)
		{
			printf("we are now in the child of %s\n", argv[0]);
		}
		else if(chld_pid < 0)
		{
			printf("Forking Error m8\n");
			execvp(argv[0], argv);
			exit(0);
		}
		else if(chld_pid<0)
		{
			printf("error with fork\n");
			exit(0);
		}
		else
		{
			waitpid(chld_pid, NULL, 0);
			printf("We are now in the parent process of %s\n", argv[0]);
			exit(0);
		}

	}

	else if(bg==1)
	{
		int chld_pid;
		chld_pid = fork();
		if (chld_pid ==0)
		{
			execvp(argv[0], argv);
			printf("\n");
			exit(0);
		}
		else if(chld_pid<0)
		{
			printf("error with fork\n");
			exit(0);
		}
		else
		{
			printf("parent process\n");
			printf("\n");
		}

	}
}

void myHandler(int sig){
	while(waitpid(-1, NULL, WNOHANG)> 0)
	{
		printf("%d\n", sig);
	}
}

void queueEntry(char *cmdline, int *count)
{
	HistoryEntry newEntry;
	*count += 1;
	printf("%d", *count);
	newEntry.cmd_num = *count;
	strcpy(newEntry.cmdline, cmdline);
	add_queue(newEntry);
}
