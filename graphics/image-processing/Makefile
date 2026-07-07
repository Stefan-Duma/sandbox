CC = gcc
SRC = src
OBJ = obj
LIB = lib
INC = include

CFLAGS = -g -Wall -Wextra -Iinclude -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

CFILES = $(wildcard ${SRC}/*.c)
OBJFILES = $(CFILES:.c=.o)

all : main

main: ${OBJFILES}
	${CC} $^ ${CFLAGS} -o $@

%.o : %.c
	${CC} -c $< ${CFLAGS} -o $@

clean:
	rm -f main ${OBJFILES}