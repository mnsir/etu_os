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

/* Функция потока */
void *func(void *args)
{
    char buffer[1024]; /* объявить буфер */
    while (FLAG != 1)  /* пока (флаг завершения потока не установлен) */
    {
        memset(buffer, 0, sizeof(buffer));        /* очистить буфер сообщения */
        if (read(FD, buffer, sizeof(buffer)) > 0) /* прочитать сообщение из именованного канала в буфер */
            printf("%s\n", buffer);               /* вывести сообщение на экран */
    }
}
/* основная программа */
int main()
{
    pthread_t th;                                    /* объявить идентификатор потока */
    VERIFY(mkfifo("foo", 0644) == 0);                /* создать именованный канал */
    FD = open("foo", O_CREAT | O_RDWR | O_NONBLOCK); /* открыть именованный канал для чтения */
    VERIFY(FD != -1);

    VERIFY(pthread_create(&th, NULL, &func, NULL) == 0); /* создать поток из функции потока */
    getchar();                                           /* ждать нажатия клавиши */
    FLAG = 1;                                            /* установить флаг завершения потока */
    VERIFY(pthread_join(th, NULL) == 0);                 /* ждать завершения потока */

    VERIFY(close(FD) == 0);     /* закрыть именованный канал */
    VERIFY(unlink("foo") == 0); /* удалить именованный канал */
}