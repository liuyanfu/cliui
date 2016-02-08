

all: main.o cliui.o cmd_table.o cli_history.o
	gcc -o cliui main.o cliui.o cmd_table.o cli_history.o
	
main.o : main.c
	gcc -Wall -g -c main.c
	
cliui.o : cliui.c
	gcc -Wall -g -c cliui.c
	
cmd_table.o : cmd_table.c ../util/list_inc.h
	gcc -Wall -g -c cmd_table.c

cli_history.o: cli_history.c cli_history.h
	gcc -Wall -g -c cli_history.c
	
clean:
	rm *.o cliui
