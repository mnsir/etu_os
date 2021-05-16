#
# build - make
# run - make run 

all: start clean progs finish

start: 
	@echo "\033[36m""build lab_5 started""\033[0m"

finish:
	@echo "\033[36m""build lab_5 finished""\033[0m"

progs:
	gcc -x c -o prog1 prog1.c -pthread
	gcc -x c -o prog2 prog2.c -pthread

clean:
	@rm -f -v bar.txt prog[12]

run:
	@echo "\033[36m""run lab_5\nwait 3 sec""\033[0m"
	@mkfifo fifo1
	@./prog1 <> fifo1 &
	@./prog2 <> fifo1 &
	@ps
	@sleep 2
	@echo '1' > fifo1
	@echo '1' > fifo1
	@sleep 1
	@ps
	@rm fifo1
	@echo "\033[36m""done\nlook at bar.txt""\033[0m"



