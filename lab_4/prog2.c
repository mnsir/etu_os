#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "sleep.h"

extern int errno;

int main(int argc, char *argv[])
{
	int child_retval;
	
	printf("%s: ", argv[0]);
	printf("process id: %08X, parent process id: %08X\n", getpid(), getppid());
	
	pid_t pid = fork();
	/* если внезапно ошибка */
	if (pid < 0)
	{
		/* TODO обработать */
		return 1;
	}
	if (pid == 0)
	{
		/* дочерний процесс */
		printf("%s: ", argv[0]);
		printf("child process id: %08X\n", getpid());
		const char* executable = "./prog1";
		execle(executable, executable, "qwe", "asd", "zxc", NULL, NULL);
		return 13;
	}
	do
	{
		sleep_ms(500);
		printf("%s: ", argv[0]);
		printf("child process still works\n");

	} while (waitpid(pid, &child_retval, WNOHANG) != pid);
	if (WIFSIGNALED(child_retval))
	{
		printf("%s: ", argv[0]);
		printf("child process was terminated by a signal %d.\n", WTERMSIG(child_retval));
	}
	else if (WIFEXITED(child_retval))
	{
		printf("%s: ", argv[0]);
		printf("child process terminated normally with code %d.\n", WEXITSTATUS(child_retval));
	}
	
	return 0;
}
