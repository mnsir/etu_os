#pragma once

#ifndef FAILED_ONLY
#define PRINT_SUCSEEDED(expr) fprintf(stderr, "SUCSEEDED: " expr "\n")
#else
#define PRINT_SUCSEEDED(expr) 
#endif

#define _VERIFY(expr, line)                                                                     \
    {                                                                                           \
        if (expr)                                                                               \
        {                                                                                       \
            PRINT_SUCSEEDED(#expr);                                                             \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            fprintf(stderr, "FAILED: " #expr ", line: %d, error: %s\n", line, strerror(errno)); \
        }                                                                                       \
    }

/* Макрос помогает мониторить ошибки */
#define VERIFY(expr) _VERIFY(expr, __LINE__)
