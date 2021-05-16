#include <time.h>      /* nanosleep */
#include <pthread.h>   /* pthread_create pthread_join */
#include <fcntl.h>     /* O_CREAT */
#include <semaphore.h> /* sem_open sem_wait sem_post sem_close sem_unlink */
#include <stdio.h>     /* printf getchar */

#include "verify.h"

int FLAG;      /* объявить флаг завершения потока */
sem_t *SEM_WR; /* объявить идентификатор семафора записи */
sem_t *SEM_RD; /* объявить идентификатор семафора чтения */
/* объявить идентификатор разделяемой памяти */
/* объявить локальный адрес */

/* функция потока */
void *func(void *args)
{
    int num;          /* объявить переменную */
    while (FLAG != 1) /* пока (флаг завершения потока не установлен) */
    {
        /* присвоить переменной случайное значение */
        printf("%d\n", num); /* вывести значение переменной на экран */
        /* скопировать значение переменной в локальный адрес */
        VERIFY(sem_post(SEM_WR) == 0);                  /* освободить семафор записи */
        VERIFY(sem_wait(SEM_RD) == 0);                  /* ждать семафора чтения */
        struct timespec ts = {.tv_nsec = 50 * 1000000}; /* задержать на время */
        nanosleep(&ts, NULL);
    }
}

/* основная программа */
int main()
{
    pthread_t th; /* объявить идентификатор потока */
    /* создать (или открыть, если существует) разделяемую память */
    /* обрезать разделяемую память до требуемого размера */
    /* отобразить разделяемую память на локальный адрес */
    SEM_WR = sem_open("sem_wr", O_CREAT, 0644, 1); /* создать (или открыть, если существует) семафор записи */
    VERIFY(SEM_WR != SEM_FAILED);
    SEM_RD = sem_open("sem_rd", O_CREAT, 0644, 1); /* создать (или открыть, если существует) семафор чтения */
    VERIFY(SEM_RD != SEM_FAILED);
    VERIFY(pthread_create(&th, NULL, &func, NULL) == 0); /* создать поток из функции потока */
    getchar();                                           /* ждать нажатия клавиши */
    FLAG = 1;                                            /* установить флаг завершения потока */
    VERIFY(pthread_join(th, NULL) == 0);                 /* ждать завершения потока */
    VERIFY(sem_close(SEM_RD) == 0);                      /* закрыть семафор чтения */
    VERIFY(sem_unlink("sem_rd") == 0);                   /* удалить семафор чтения */
    VERIFY(sem_close(SEM_WR) == 0);                      /* закрыть семафор записи */
    VERIFY(sem_unlink("sem_wr") == 0);                   /* удалить семафор записи */
    /* закрыть отображение разделяемой памяти на локальный адрес */
    /* удалить разделяемую память */
}