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
		struct timespec t = { .tv_sec = 2 };
		nanosleep(&t, NULL);
		exit(0);
	}
	do
	{
		nanosleep(&ts, NULL);
		printf("wait\n");

	} while (waitpid(pid, &child_retval, WNOHANG) != pid);
	printf("ok\n");
	return 0;
}
