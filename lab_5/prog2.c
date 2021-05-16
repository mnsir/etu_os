#include <stdio.h>  
#include <errno.h>  
#include <string.h> 
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define FAILED_ONLY
#include "verify.h"

#define THE_CHAR '1'

int FLAG;   /* объявить флаг завершения потока */
sem_t *SEM; /* объявить идентификатор именованного семафора */
int FD;     /* объявить дескриптор файла */

void zzz(int ms)
{
    struct timespec ts = {.tv_nsec = ms * 1000000};
    nanosleep(&ts, NULL);
}

/* функция потока */
void *func(void *args)
{
    const char ch = THE_CHAR; /* объявить переменную типа символ и присвоить ей значение */
    while (FLAG != 1)         /* пока (флаг завершения потока не установлен) */
    {
        VERIFY(sem_wait(SEM) == 0); /* захватить именованный семафор */
        for (int i = 0; i < 8; ++i) /* в цикле несколько раз выполнять */
        {
            VERIFY(write(FD, &ch, 1) != -1); /* выводить символ в файл */
            zzz(10);                         /* задержать на время */
        }
        VERIFY(write(FD, "\n", 1) != -1);
        VERIFY(sem_post(SEM) == 0); /* освободить именованный семафор */
        zzz(10);                    /* задержать на время */
    }
}

/* основная программа */
int main()
{
    SEM = sem_open("foo", O_CREAT, 0644, 1); /* создать (или открыть, если существует) именованный семафор */
    VERIFY(SEM != SEM_FAILED);
    FD = open("bar.txt", O_CREAT | O_WRONLY | O_APPEND, 0644, 1); /* создать (или открыть, если существует) файл */
    VERIFY(FD != -1);
    pthread_t th;
    VERIFY(pthread_create(&th, NULL, &func, NULL) == 0); /* создать поток из функции потока */
    getchar();                                           /* ждать нажатия клавиши */
    FLAG = 1;                                            /* установить флаг завершения потока */
    VERIFY(pthread_join(th, NULL) == 0);                 /* ждать завершения потока */
    VERIFY(close(FD) == 0);                              /* закрыть файл */
    VERIFY(sem_close(SEM) == 0);                         /* закрыть именованный семафор; */
    VERIFY(sem_unlink("foo") == 0);                      /* удалить именованный семафор */
    return 0;
}