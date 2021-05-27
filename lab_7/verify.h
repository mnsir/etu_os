#pragma once

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef FAILED_ONLY
#define PRINT_SUCSEEDED(expr) fprintf(stderr, "SUCSEEDED: " expr "\n")
#else
#define PRINT_SUCSEEDED(expr)
#endif

#define _VERIFY(expr, file, line)                                        \
    {                                                                    \
        if (expr)                                                        \
        {                                                                \
            PRINT_SUCSEEDED(#expr);                                      \
        }                                                                \
        else                                                             \
        {                                                                \
            fprintf(stderr,                                              \
                    "FAILED: " #expr ", file: %s line: %d, error: %s\n", \
                    file,                                                \
                    line,                                                \
                    strerror(errno));                                    \
        }                                                                \
    }

/* Макрос помогает мониторить ошибки */
#define VERIFY(expr) _VERIFY(expr, __FILE__, __LINE__)
