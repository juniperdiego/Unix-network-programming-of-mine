all:
	gcc pr_cpu_time.c tcp_listen.c web_child.c serv0.c -g -Wall -o serv0
	gcc client.c -g -Wall -o cli 
serv:
	gcc pr_cpu_time.c tcp_listen.c web_child.c serv0.c -g -Wall -o serv0 
cli:
	gcc client.c -g -Wall -o cli01 -lpthread -static
