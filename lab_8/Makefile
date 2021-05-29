name="lab_8"
all: start clean build finish

start: 
	@echo "\033[36m""build $(name) started""\033[0m"

finish:
	@echo "\033[36m""build $(name) finished""\033[0m"

build:
	gcc -x c -o prog1 prog1.c -pthread
	gcc -x c -o prog2 prog2.c -pthread

clean:
	@rm -f -v prog[12]

run:
	@echo "\033[36m""run2 $(name)\nwait 3 sec""\033[0m"
	@mkfifo fifo > 1
	@./prog1 <> fifo &
	@./prog2 <> fifo &
	@ps
	@sleep 3
	@echo '2' > fifo
	@echo '1' > fifo
	@sleep 1
	@ps
	@rm fifo
	@echo "\033[36m""done\n""\033[0m"