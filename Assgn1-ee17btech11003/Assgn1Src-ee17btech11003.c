#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<sys/mman.h>
static struct timeval *t0,*t1;

int main(int argc, char const *argv[])
{
	char *command=malloc(sizeof(char));
	*command='\0';
	for(int i=1;i<argc;i++)
	{	
		command=realloc(command,strlen(command)+strlen(argv[i])+1);
		strcat(command,argv[i]);
	    strcat(command," ");
	}   
	t0 = mmap(NULL, sizeof(struct timeval *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	t1 = mmap(NULL, sizeof(struct timeval *), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	pid_t pid1;
	pid1=fork();	
	if(pid1<0)
	{
		printf("forking failed");
		return 1;
	}
	else if(pid1==0)
	{
		gettimeofday(t0, 0);
		execlp("/bin/sh","sh", "-c", command,(char*)0);
	}
	else if(pid1>0)
	{
		wait(NULL);
		gettimeofday(t1, 0);
		float elapsed = (t1->tv_sec-t0->tv_sec) + (t1->tv_usec-t0->tv_usec)/1000000.0;
		printf("Elapsed time: %f\n", elapsed);
        munmap(t0, sizeof(struct timeval *));
        munmap(t1, sizeof(struct timeval *));
	}
	free(command);
	return 0;
}