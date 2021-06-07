#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <pthread.h>

#define DISABLE_SUCSEEDED
#define ENABLE_COLORED_RESULT
#include "verify.h"

typedef struct args_tag
{
    struct sockaddr *sa_ptr;
    socklen_t sa_size;
    int flag;
    int sfd; /* объявить идентификатор «слушающего» сокета */
    int mfd;
} args_t;

typedef struct msgbuf
{
    long type;
    char text[1024];
} msg_t;

void zzz(int ms)
{
    struct timespec ts = {.tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000};
    nanosleep(&ts, NULL);
}

/* функция приема запросов */
void *Receive(void *args)
{
    args_t *args_ptr = (args_t *)args;
    msg_t msg = {0};
    /* пока (флаг завершения потока приема не установлен) */
    while (args_ptr->flag)
    {
        /* принять запрос из сокета */
        if (recvfrom(args_ptr->sfd,
                     msg.text,
                     sizeof msg.text,
                     MSG_DONTWAIT,
                     args_ptr->sa_ptr,
                     &args_ptr->sa_size) > 0)
        {
            /* положить запрос в очередь на обработку */
            msg.type = 1;
            msgsnd(args_ptr->mfd, &msg, strlen(msg.text), IPC_NOWAIT);
            memset(msg.text, 0, sizeof msg.text);
        }
    }
}

/* функция обработки запросов */
void *Process(void *args)
{
    args_t *args_ptr = (args_t *)args;
    msg_t msg = {0};
    /* пока (флаг завершения потока обработки не установлен) */
    while (args_ptr->flag)
    {
        /* прочитать запрос из очереди на обработку */
        if (msgrcv(args_ptr->mfd, &msg, sizeof msg.text, 1, IPC_NOWAIT) > 0)
        {
            /* обработать запрос и сформировать ответ */
            if (strcmp(msg.text, "getaddrinfo") == 0)
            {
                const char *site = "google.com";
                struct addrinfo *result;
                if (getaddrinfo(site, NULL, NULL, &result) == 0)
                {
                    struct sockaddr_in *in = (struct sockaddr_in *)result->ai_addr;
                    sprintf(msg.text, "site: %s, address: %s:%d", site, inet_ntoa(in->sin_addr), in->sin_port);
                }
                else
                {
                    strcpy(msg.text, "getaddrinfo error");
                }
            }
            else
            {
                strcpy(msg.text, "Error! Wrong Request!");
            }
            /* положить ответ в очередь на передачу */
            msg.type = 2;
            msgsnd(args_ptr->mfd, &msg, sizeof msg.text, IPC_NOWAIT);
            memset(msg.text, 0, sizeof msg.text);
        }
    }
}

/* функция передачи ответов */
void *Send(void *args)
{
    args_t *args_ptr = (args_t *)args;
    msg_t msg = {0};
    /* пока (флаг завершения потока передачи не установлен) */
    while (args_ptr->flag)
    {
        /* прочитать ответ из очереди на передачу */
        if (msgrcv(args_ptr->mfd, &msg, sizeof msg.text, 2, IPC_NOWAIT) > 0)
        {
            /* передать ответ в сокет */
            sendto(args_ptr->sfd,
                   msg.text,
                   sizeof msg.text,
                   MSG_CONFIRM,
                   args_ptr->sa_ptr,
                   args_ptr->sa_size);
            memset(&msg.text, 0, sizeof msg.text);
        }
    }
}

/* основная программа */
int main()
{
    /* объявить идентификатор потока приема обработки передачи ответов */
    pthread_t thReceive, thProcess, thSend;

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8080);

    args_t args;
    args.sa_ptr = (struct sockaddr *)&servaddr;
    args.sa_size = sizeof servaddr;
    args.flag = 1;
    args.sfd = socket(AF_INET, SOCK_DGRAM, 0); /* создать «слушающий» сокет */
    VERIFY(args.sfd != -1);
    /* привязать «слушающий» сокет к адресу */
    VERIFY(bind(args.sfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == 0);

    /* создать очередь запросов на обработку */
    args.mfd = msgget(12, IPC_CREAT | 0644);
    VERIFY(args.mfd != -1);
    /* создать очередь ответов на передачу */

    /* создать поток приема обработки передачи ответов */
    VERIFY(pthread_create(&thReceive, NULL, Receive, (void *)&args) == 0);
    VERIFY(pthread_create(&thProcess, NULL, Process, (void *)&args) == 0);
    VERIFY(pthread_create(&thSend, NULL, Send, (void *)&args) == 0);

    /* ждать нажатия клавиши */
    zzz(4000);
    /* установить флаг завершения потока приема обработки передачи ответов */
    args.flag = 0;

    /* ждать завершения потока приема обработки передачи ответов */
    VERIFY(pthread_join(thReceive, NULL) == 0);
    VERIFY(pthread_join(thProcess, NULL) == 0);
    VERIFY(pthread_join(thSend, NULL) == 0);

    /* закрыть соединение с клиентом */
    VERIFY(msgctl(args.mfd, IPC_RMID, NULL) == 0);
    /* закрыть сокет для работы с клиентом */
    close(args.sfd); /* закрыть «слушающий» сокет */
}
