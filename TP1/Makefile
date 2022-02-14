CC = gcc
INC = -I/usr/local/inc -I/usr/local/include/ejovo/matrix -I/usr/local/include/ejovo
LIB = -lejovo -lm
SRC = qtest.c

compile: $(SRC)
	$(CC) $(SRC) -o q $(LIB) $(INC) -static -static-libgcc