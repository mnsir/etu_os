#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
	pid_t id = getpid();
	pid_t parent_id = getppid();
	printf("this proc id: %X, parent proc id: %X\n", id, parent_id);
	struct timespec ts = { .tv_sec = 2 };
	for (int i = 1; i < argc; ++i)
	{
		nanosleep(&ts, NULL);
		printf("arg %d: %s\n", i, argv[i]);
	}
	return 0;
}
