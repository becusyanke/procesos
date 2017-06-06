#Archivo makefile simple

PROYECTO=tarea2
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LDFLAGS=-lm -lpthread
CC=gcc
SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)

all: $(PROYECTO)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

$(PROYECTO): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: clean

install: all
	if [ ! -d $(DESTDIR) ]; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)

unistall:
	sudo rm $(DESTDIR)/$(PROYECTO)

clean: 
	rm -f *.o muestreo
