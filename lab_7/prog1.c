#include <sys/types.h> /* mkfifo */
#include <sys/stat.h>  /* mkfifo open */
#include <fcntl.h>     /* open */
#include <unistd.h>    /* close unlink */
#include <time.h>
#include <pthread.h>
#define ENABLE_COLORED_RESULT
#include "verify.h"

int FLAG; /* объявить флаг завершения потока */
int FD;   /* объявить дескриптор именованного канала */

/* функция потока */
void *func(void *args)
{
    char buffer[32];  /* объявить буфер */
    while (FLAG != 1) /* пока (флаг завершения потока не установлен) */
    {
        strcpy(buffer, "hello, world!");                 /* сформировать сообщение в буфере */
        write(FD, buffer, strlen(buffer));               /* записать сообщение из буфера в именованный канал */
        struct timespec ts = {.tv_nsec = 200 * 1000000}; /* задержать на время */
        nanosleep(&ts, NULL);
    }
}

/* основная программа */
int main()
{
    pthread_t th;                                    /* объявить идентификатор потока */
    VERIFY(mkfifo("foo", 0644) == 0);                /* создать именованный канал */
    FD = open("foo", O_CREAT | O_RDWR | O_NONBLOCK); /* открыть именованный канал для записи */
    VERIFY(FD != -1);                                /* открыть именованный канал для записи */

    VERIFY(pthread_create(&th, NULL, &func, NULL) == 0); /* создать поток из функции потока */
    getchar();                                           /* ждать нажатия клавиши */
    FLAG = 1;                                            /* установить флаг завершения потока */
    VERIFY(pthread_join(th, NULL) == 0);                 /* ждать завершения потока */

    VERIFY(close(FD) == 0);     /* закрыть именованный канал */
    VERIFY(unlink("foo") == 0); /* удалить именованный канал */
}