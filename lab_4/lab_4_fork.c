#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern int errno;

int main()
{
	int child_retval;
	struct timespec ts = { .tv_nsec = 500 * 1000000 };
	
	printf("me: %#010X, parent: %#010X\n", getpid(), getppid());
	
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
		printf("child: %#010X\n", getpid());
		struct timespec t = { .tv_sec = 2 };
		nanosleep(&t, NULL);
		return 0;
	}
	do
	{
		nanosleep(&ts, NULL);
		printf("wait\n");

	} while (waitpid(pid, &child_retval, WNOHANG) != pid);
	if (WIFSIGNALED(child_retval))
	{
		printf("child process was terminated by a signal %d\n", WTERMSIG(child_retval));
	}
	else if (WIFEXITED(child_retval))
	{
		printf("child process exit code: %d\n", WEXITSTATUS(child_retval));
	}
	
	return 0;
}
