

int FLAG; /* объявить флаг завершения потока */
/* объявить идентификатор семафора записи */
/* объявить идентификатор семафора чтения */
/* объявить идентификатор разделяемой памяти */
/* объявить локальный адрес */

/* функция потока */
void *func(void *args)
{
    /* объявить переменную */
    while (FLAG != 1) /* пока (флаг завершения потока не установлен) */
    {
        /* присвоить переменной случайное значение */
        /* вывести значение переменной на экран */
        /* скопировать значение переменной в локальный адрес */
        /* освободить семафор записи */
        /* ждать семафора чтения */
        /* задержать на время */
    }
}

/* основная программа */
int main()
{
    /* объявить идентификатор потока */
    /* создать (или открыть, если существует) разделяемую память */
    /* обрезать разделяемую память до требуемого размера */
    /* отобразить разделяемую память на локальный адрес */
    /* создать (или открыть, если существует) семафор записи */
    /* создать (или открыть, если существует) семафор чтения */
    /* создать поток из функции потока */
    /* ждать нажатия клавиши */
    /* установить флаг завершения потока */
    /* ждать завершения потока */
    /* закрыть семафор чтения */
    /* удалить семафор чтения */
    /* закрыть семафор записи */
    /* удалить семафор записи */
    /* закрыть отображение разделяемой памяти на локальный адрес */
    /* удалить разделяемую память */
}