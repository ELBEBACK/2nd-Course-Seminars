.PHONY : all run clean 

CC = gcc
CFLAGS = -Wall -Wextra

NAMEEXE := 3workers-right-answer
OBJFILES := 3workers-right-answer.o safe-semaphors.o

all: $(NAMEEXE)

run: $(NAMEEXE)
	./$(NAMEEXE) 3 4 5 

$(NAMEEXE) : $(OBJFILES) safe-semaphors.h
	$(CC) $(OBJFILES) -o $@ 

%.o : %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean: 
	rm $(NAMEEXE) *.o 