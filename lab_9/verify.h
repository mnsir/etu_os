#pragma once

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef ENABLE_COLORED_RESULT
#define RESULT_SUCSEEDED "\x1b[102m \x1b[0m "
#define RESULT_FAILED "\x1b[101m \x1b[0m "
#else
#define RESULT_SUCSEEDED "SUCSEEDED: "
#define RESULT_FAILED "FAILED: "
#endif

#ifdef DISABLE_SUCSEEDED
#define PRINT_SUCSEEDED(expr)
#else
#define PRINT_SUCSEEDED(expr) fprintf(stderr, RESULT_SUCSEEDED expr "\n")
#endif

#ifdef DISABLE_FAILED
#define PRINT_FAILED(expr, file, line)
#else
#define PRINT_FAILED(expr, file, line)                             \
    fprintf(stderr,                                                \
            RESULT_FAILED expr ", file: %s line: %d, error: %s\n", \
            file,                                                  \
            line,                                                  \
            strerror(errno));
#endif

#define _VERIFY(expr, file, line)            \
    do                                       \
    {                                        \
        if (expr)                            \
        {                                    \
            PRINT_SUCSEEDED(#expr);          \
        }                                    \
        else                                 \
        {                                    \
            PRINT_FAILED(#expr, file, line); \
        }                                    \
    } while (0)

/* Макрос помогает мониторить ошибки */
#define VERIFY(expr) _VERIFY(expr, __FILE__, __LINE__)
