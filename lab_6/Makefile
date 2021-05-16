all: start clean build finish

start: 
	@echo "\033[36m""build lab_6 started""\033[0m"

finish:
	@echo "\033[36m""build lab_6 finished""\033[0m"

build:
	gcc -x c -o prog1 prog1.c -pthread
	gcc -x c -o prog2 prog2.c -pthread

clean:
	@rm -f -v prog[12]