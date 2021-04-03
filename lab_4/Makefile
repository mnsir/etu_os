CC=gcc

all: clean prog[23]

prog2: prog1
	$(CC) -x c -o prog2 prog2.c

prog3: prog1
	$(CC) -x c -o prog3 prog3.c

prog1:
	$(CC) -x c -o prog1 prog1.c

clean:
	rm prog[123]
