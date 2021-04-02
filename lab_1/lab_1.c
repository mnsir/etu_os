#include <stdio.h>
#include <time.h>
#include <pthread.h>

/* Задержка в мс */
void sleep(int ms) {
    timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

/* Данные потока */
typedef struct args_tag {
	int flag;	/* флаг */
	int counter;	/* счетчик */
	int delay;	/* задержка */
} args_t;

/* Функция потока */
/* Пока флаг = 0, увеличиваем счетчик потока на 1 каждые delay милисекунд */
void* func(void* args) {
	args_t* args_ptr = (args_t*)args;
	while (args_ptr->flag) {
        	sleep(args_ptr->delay);
        	++args_ptr->counter;
	}
	return 0;
}

int main() {
	/* Количество потоков берется из количества задержек */
	const int delays[] = { 30, 70, 110 };
	const int n = sizeof(delays)/sizeof(int);
	printf("threads count = %d\n", n);
	pthread_t th[n];
	/* Инициализировать данные потоков */
	args_t args[n];
	for (int i = 0; i < n; ++i)
		args[i] = { .flag = 1, .counter = 0, .delay = delays[i] };
	/* Создать потоки */
	for (int i = 0; i < n; ++i)
		pthread_create(&th[i], NULL, func, (void*)&args[i]);
	/* Выводить данные счетчиков каждые 100мс */
	for (int t = 0; t < 20; ++t) {
		sleep(100);
		for (int i = 0; i < n; ++i)
			printf("%d: %d\t", i, args[i].counter);
		printf("\n");
	}
	/* Установить все флаги в 0 */
	for (int i = 0; i < n; ++i)
		args[i].flag = 0;
	/* Завершить потоки */
	for (int i = 0; i < n; ++i)
		pthread_join(th[i], NULL);
	return 0;
}
