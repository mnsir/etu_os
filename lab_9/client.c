#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
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
    int sfd; /* объявить сокет для работы с сервером */
} args_t;

void zzz(int ms)
{
    struct timespec ts = {.tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000};
    nanosleep(&ts, NULL);
}

/* функция передачи запросов */
void *Send(void *args)
{
    args_t *args_ptr = (args_t *)args;
    size_t buffer_size = 32;
    char buffer[buffer_size];
    while (args_ptr->flag) /* пока (флаг завершения потока передачи запросов не установлен) */
    {
        /* задержка на время */
        zzz(500);
        strcpy(buffer, "getaddrinfo"); /* создать запрос */
        /* передать запрос в сокет */
        sendto(args_ptr->sfd,
               buffer,
               buffer_size,
               MSG_CONFIRM,
               args_ptr->sa_ptr,
               args_ptr->sa_size);
    }
}

/* функция приема ответов */
void *Receive(void *args)
{
    args_t *args_ptr = (args_t *)args;

    size_t buffer_size = 1024;
    char buffer[buffer_size];
    int n;

    while (args_ptr->flag) /* пока (флаг завершения потока приема ответов не установлен) */
    {
        memset(buffer, 0, buffer_size);
        /* принять ответ из сокета */
        if (recvfrom(args_ptr->sfd,
                     buffer,
                     buffer_size,
                     MSG_DONTWAIT,
                     args_ptr->sa_ptr,
                     &args_ptr->sa_size) > 0)
        {
            /* вывести ответ на экран */
            printf("Client: %s\n", buffer);
        }
    }
}

/* основная функция */
int main()
{
    /* объявить идентификатор потока передачи запросов, приема ответов */
    pthread_t thSend, thReceive;

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    args_t args;
    args.sa_ptr = (struct sockaddr *)&servaddr;
    args.sa_size = sizeof servaddr;
    args.flag = 1;
    args.sfd = socket(AF_INET, SOCK_DGRAM, 0); /* создать сокет для работы с сервером */
    VERIFY(args.sfd != -1);

    /* создать поток передачи запросов, приема ответов */
    VERIFY(pthread_create(&thSend, NULL, Send, (void *)&args) == 0);
    VERIFY(pthread_create(&thReceive, NULL, Receive, (void *)&args) == 0);

    /* ждать нажатия клавиши */
    zzz(3000);
    /* установить флаг завершения потока передачи запросов, приема ответов */
    args.flag = 0;

    /* ждать завершения потока передачи запросов, приема ответов */
    VERIFY(pthread_join(thSend, NULL) == 0);
    VERIFY(pthread_join(thReceive, NULL) == 0);

    /* закрыть соединение с сервером */
    close(args.sfd); /* закрыть сокет */
}
