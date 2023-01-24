SRCMODULES = dynamic_word.c launch_prog.c new_line.c redirection.c switch.c word_list.c

OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -Wall -g

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

prog: main_prog.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

run: prog
	./prog

clean:
	rm -f *.o prog

