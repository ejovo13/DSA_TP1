CC = gcc
# INC = -I/usr/local/inc
# LIB = -L/usr/local/lib
LIB = -lejovo -lm
SRC = qtest.c

compile: $(SRC)
	$(CC) $(SRC) -o q $(LIB) -g