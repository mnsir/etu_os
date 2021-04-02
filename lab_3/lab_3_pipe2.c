#include <fcntl.h>
#define _GNU_SOURCE
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

/* объявить флаг завершения потока 1; */
int FLAG_1;
/* объявить флаг завершения потока 2; */
int FLAG_2;
/* объявить идентификатор неименованного канала; */
int FD[2];

int pipe2(int[2], int);

/* функция потока 1() */
void* function1(void* args)
{
	/* объявить буфер; */
	char buffer[32];
	/* пока(флаг завершения потока 1 не установлен) */
	while (FLAG_1 != 1)
	{
		/* сформировать сообщение в буфере; */
		strcpy(buffer, "hello, world!");
		/* записать сообщение из буфера в неименованный канал; */
		write(FD[1], buffer, strlen(buffer));
		/* задержать на время; */
		struct timespec ts = { .tv_sec = 1 };
		nanosleep(&ts, NULL);
	}
}

/* функция потока 2() */
void* function2(void* args)
{
	/* объявить буфер; */
	char buffer[1024];
	/* пока(флаг завершения потока 2 не установлен) */
	while (FLAG_2 != 1)
	{
		/* очистить буфер; */
		memset(buffer, 0, sizeof(buffer));
		/* прочитать сообщение из неименованного канала в буфер; */
		if (read(FD[0], buffer, sizeof(buffer)) > 0)
		{
			/* вывести сообщение на экран; */
			printf("%s\n", buffer);
		}
	}
}

/* основная программа() */
int main()
{
	/* объявить идентификатор потока 1; */
	pthread_t thread1;
	/* объявить идентификатор потока 2; */
	pthread_t thread2;
	/* создать неименованный канал; */
	pipe2(FD, O_NONBLOCK);
	/* создать поток из функции потока 1; */
	pthread_create(&thread1, NULL, function1, NULL);
	/* создать поток из функции потока 2; */
	pthread_create(&thread2, NULL, function2, NULL);
	/* ждать нажатия клавиши; */
	getchar();
	/* установить флаг завершения потока 1; */
	FLAG_1 = 1;
	/* установить флаг завершения потока 2; */
	FLAG_2 = 1;
	/* ждать завершения потока 1; */
	pthread_join(thread1, NULL);
	/* ждать завершения потока 2; */
	pthread_join(thread2, NULL);
	/* закрыть неименованный канал; */
	close(FD[0]);
	close(FD[1]);
}

