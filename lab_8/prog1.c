#include <pthread.h>
#define ENABLE_COLORED_RESULT
#include "verify.h"

int FLAG; /* объявить флаг завершения потока */
/* объявить идентификатор очереди сообщений */

/* Функция потока */
void *func(void *args)
{
    /* объявить буфер */
    while (FLAG != 1) /* пока (флаг завершения потока не установлен) */
    {
        /* сформировать сообщение в буфере */
        /* записать сообщение из буфера в очередь сообщений */
        struct timespec ts = {.tv_nsec = 200 * 1000000}; /* задержать на время */
        nanosleep(&ts, NULL);
    }
}

/* основная программа */
int main()
{
    pthread_t th; /* объявить идентификатор потока */
                  /* создать (или открыть, если существует) очередь сообщений */

    VERIFY(pthread_create(&th, NULL, &func, NULL) == 0); /* создать поток из функции потока */
    getchar();                                           /* ждать нажатия клавиши */
    FLAG = 1;                                            /* установить флаг завершения потока */
    VERIFY(pthread_join(th, NULL) == 0);                 /* ждать завершения потока */

    /* закрыть очередь сообщений */
    /* удалить очередь сообщений */
}