#include <sys/types.h> /* ftok */
#include <sys/ipc.h>   /* ftok msgget msgctl */
#include <sys/msg.h>   /* msgget msgctl */
#include <pthread.h>
#define ENABLE_COLORED_RESULT
#include "verify.h"
#include "common.h"

int FLAG; /* объявить флаг завершения потока */
int ID;   /* объявить идентификатор очереди сообщений */

/* Функция потока */
void *func(void *args)
{
    msg_t msg = {.type = 1}; /* объявить буфер */
    while (FLAG != 1)        /* пока (флаг завершения потока не установлен) */
    {
        memset(msg.text, 0, sizeof msg.text);              /* очистить буфер сообщения */
        if (msgrcv(ID, &msg, sizeof msg.text, 1, 0) != -1) /* принять сообщение из очереди сообщений в буфер */
            printf("%s\n", msg.text);                      /* вывести сообщение на экран */
    }
}

/* основная программа */
int main()
{
    pthread_t th;                       /* объявить идентификатор потока */
    ID = msgget(KEY, IPC_CREAT | 0644); /* создать (или открыть, если существует) очередь сообщений */
    VERIFY(ID != -1);

    VERIFY(pthread_create(&th, NULL, &func, NULL) == 0); /* создать поток из функции потока */
    getchar();                                           /* ждать нажатия клавиши */
    FLAG = 1;                                            /* установить флаг завершения потока */
    VERIFY(pthread_join(th, NULL) == 0);                 /* ждать завершения потока */

    /* закрыть очередь сообщений */
    VERIFY(msgctl(ID, IPC_RMID, NULL) == 0); /* удалить очередь сообщений */
}