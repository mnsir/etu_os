#pragma once

#define KEY 12

typedef struct msgbuf
{
    long type;
    char text[128];
} msg_t;