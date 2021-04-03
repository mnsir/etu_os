#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "sleep.h"

#define EXEC "./prog1"

static int func(void*);

int main(int argc, char *argv[])
{
	int child_retval;

	printf("%s: ", argv[0]);
	printf("process id: %08X, parent process id: %08X\n", getpid(), getppid());

	size_t size = 1024 * 1024;
	void* stack_ptr = malloc(size);
	int pid = clone(func, stack_ptr + size, SIGCHLD, (void*)argv[0]);

	if (pid == -1)
	{
		printf("%s: ", argv[0]);
		printf("clone error\n");
		return 1;
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

static int func(void* arg)
{
	printf("%s: ", (const char* const)arg);
	printf("child process id: %08X\n", getpid());
	execle(EXEC, EXEC, "qwe", "asd", "zxc", NULL, NULL);
	return 13;
}
