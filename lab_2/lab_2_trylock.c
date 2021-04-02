#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

#define SLEEP() nanosleep(&TS, NULL)
#ifdef SLEEP
struct timespec TS = { .tv_nsec = 100 * 1000000 };
#endif // SLEEP

/* объявить флаг завершения потока 1; */
int FLAG_1;
/* объявить флаг завершения потока 2; */
int FLAG_2;
/* объявить идентификатор мьютекса; */
pthread_mutex_t mutex;

#define FUNCTION(num) function##num
#define DECLARE_THREAD_FUNCTION(num)                                            \
/* функция потока num */                                                        \
void* FUNCTION(num)(void* args)                                                 \
{                                                                               \
        while (FLAG_1 != 1)                                                     \
        {                                                                       \
                /* захватить мьютекс; */                                        \
                if (EBUSY != pthread_mutex_trylock(&mutex)) {                   \
                /* в цикле несколько раз выполнять */                           \
                        for (int i = 0; i < 10; ++i)                            \
                        {                                                       \
                                /* выводить символ ‘num’ на экран; */           \
                                printf(#num);                                   \
                                /* задержать на время; */                       \
                                SLEEP();                                        \
                        }                                                       \
                        fflush(stdout);                                         \
                        /* освободить мьютекс; */                               \
                        pthread_mutex_unlock(&mutex);                           \
                }                                                               \
                /* задержать на время; */                                       \
                SLEEP();                                                        \
        }                                                                       \
}

DECLARE_THREAD_FUNCTION(1)
DECLARE_THREAD_FUNCTION(2)

/* основная программа */
int main()
{
        printf("PRESS 'ENTER' TO STOP IT!\n");
        /* объявить идентификатор потока 1; */
        pthread_t thread1;
        /* объявить идентификатор потока 2; */
        pthread_t thread2;
        /* инициализировать мьютекс; */
        pthread_mutex_init(&mutex, NULL);
        /* создать поток из функции потока 1; */
        pthread_create(&thread1, NULL, FUNCTION(1), NULL);
        /* создать поток из функции потока 2; */
        pthread_create(&thread2, NULL, FUNCTION(2), NULL);
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
        /* удалить мьютекс; */
        pthread_mutex_destroy(&mutex);
}


