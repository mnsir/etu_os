name="lab_9"
all: start clean build finish

start: 
	@echo "\033[36m""build $(name) started""\033[0m"

finish:
	@echo "\033[36m""build $(name) finished""\033[0m"

build:
	gcc -x c -o server server.c -pthread
	gcc -x c -o client client.c -pthread

clean:
	@rm -f -v server client

run:
	@echo "\033[36m""run $(name)\n""\033[0m"
	@./server &
	@./client &
	@ps
	@sleep 6
	@ps
	@echo "\033[36m""done\n""\033[0m"