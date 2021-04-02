#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

/* Возвести в квадрат */
void* pwr2(void* arg) {
    return (void*)((intptr_t)arg * (intptr_t)arg);
}

int main() {
    char again;		/* Повтор */
    intptr_t num;	/* Выделить память под число, чтоб было размера void* */
    pthread_t th;	/* Поток */
    /* Цикл */
    do {
	/* Приглашение TODO нет защиты от некорректного ввода */
        printf ("Ввод: ");
        scanf ("%" SCNiPTR, &num);
        
        if (pthread_create(&th, NULL, pwr2, (void*)num) != 0) {
            printf ("Произошла ошибка создания потока!\n");
        }
        else if (pthread_join(th, (void**)&num) != 0) {
            printf ("Произошла ошибка синхронизации!\n");
        }
        else {
            printf ("Результат: %" PRIiPTR "\n", num);
        }
        /* Приглашение повторить */
        printf ("Повторить? Y/N: ");
        scanf (" %c", &again);
    } while (again == 'y' || again == 'Y');
    /* Выход */
    return 0;
}

