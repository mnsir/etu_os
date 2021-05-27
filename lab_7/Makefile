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

run:
	@echo "\033[36m""run lab_6\nwait 5 sec""\033[0m"
	@mkfifo fifo1
	@mkfifo fifo2
	@./prog1 <> fifo1 &
	@./prog2 <> fifo2 &
	@ps
	@sleep 5
	@echo '2' > fifo2
	@echo '1' > fifo1
	@sleep 1
	@ps
	@rm fifo[12]
	@echo "\033[36m""done\nlook at bar.txt""\033[0m"