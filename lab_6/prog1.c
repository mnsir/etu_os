#include <time.h>      /* nanosleep */
#include <pthread.h>   /* pthread_create pthread_join */
#include <fcntl.h>     /* O_CREAT */
#include <semaphore.h> /* sem_open sem_wait sem_post sem_close sem_unlink */
#include <stdio.h>     /* printf getchar */
#include <stdlib.h>    /* srand rand */
#include <sys/types.h> /* key_t */
#include <sys/ipc.h>   /* ftok */
#include <sys/shm.h>   /* shmget shmat shmdt shmctl */

#define FAILED_ONLY
#include "verify.h"

#define SEM_WR_NAME "sem_wr"
#define SEM_RD_NAME "sem_rd"

int FLAG;      /* объявить флаг завершения потока */
sem_t *SEM_WR; /* объявить идентификатор семафора записи */
sem_t *SEM_RD; /* объявить идентификатор семафора чтения */
int ID;        /* объявить идентификатор разделяемой памяти */
void *PTR;     /* объявить локальный адрес */

/* функция потока */
void *func(void *args)
{
    int num; /* объявить переменную */
    srand(time(NULL));
    while (FLAG != 1) /* пока (флаг завершения потока не установлен) */
    {
        num = rand();                /* присвоить переменной случайное значение */
        printf("sended: %d\t", num); /* вывести значение переменной на экран */
        fflush(stdout);
        *(int *)PTR = num;                               /* скопировать значение переменной в локальный адрес */
        VERIFY(sem_post(SEM_WR) == 0);                   /* освободить семафор записи */
        VERIFY(sem_wait(SEM_RD) == 0);                   /* ждать семафора чтения */
        struct timespec ts = {.tv_nsec = 200 * 1000000}; /* задержать на время */
        nanosleep(&ts, NULL);
    }
}

/* основная программа */
int main()
{
    pthread_t th; /* объявить идентификатор потока */

    key_t key = ftok("prog1.c", 1); /* создать (или открыть, если существует) разделяемую память */
    VERIFY(key != -1);
    ID = shmget(key, sizeof(int), 0644 | IPC_CREAT); /* изменить размер разделяемой памяти на требуемый */
    VERIFY(ID != -1);
    PTR = shmat(ID, NULL, 0); /* отобразить разделяемую память на локальный адрес */
    VERIFY(PTR != (void *)-1);

    SEM_WR = sem_open(SEM_WR_NAME, O_CREAT, 0644, 1); /* создать (или открыть, если существует) семафор записи */
    VERIFY(SEM_WR != SEM_FAILED);
    SEM_RD = sem_open(SEM_RD_NAME, O_CREAT, 0644, 1); /* создать (или открыть, если существует) семафор чтения */
    VERIFY(SEM_RD != SEM_FAILED);

    VERIFY(pthread_create(&th, NULL, &func, NULL) == 0); /* создать поток из функции потока */
    getchar();                                           /* ждать нажатия клавиши */
    FLAG = 1;                                            /* установить флаг завершения потока */
    VERIFY(pthread_join(th, NULL) == 0);                 /* ждать завершения потока */

    VERIFY(sem_close(SEM_RD) == 0);       /* закрыть семафор чтения */
    VERIFY(sem_unlink(SEM_RD_NAME) == 0); /* удалить семафор чтения */
    VERIFY(sem_close(SEM_WR) == 0);       /* закрыть семафор записи */
    VERIFY(sem_unlink(SEM_WR_NAME) == 0); /* удалить семафор записи */

    VERIFY(shmdt(PTR) != -1);                 /* закрыть отображение разделяемой памяти на локальный адрес */
    VERIFY(shmctl(ID, IPC_RMID, NULL) != -1); /* удалить разделяемую память */
}