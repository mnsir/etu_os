#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	printf("%s: ", argv[0]);
	if (argc < 2)
	{
		printf("no arguments to print\n");
		return 1;
	}
	printf("process id: %08X, parent process id: %08X\n", getpid(), getppid());
	struct timespec ts = { .tv_sec = 2 };
	for (int i = 1; i < argc; ++i)
	{
		nanosleep(&ts, NULL);
		printf("%s: ", argv[0]);
		printf("argument %d is %s\n", i, argv[i]);
	}
	return 0;
}
