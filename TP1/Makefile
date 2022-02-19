# This make file is configured for my system and will **NOT** work on another system
# For this reason, I've provided the binary ./q as a part of the git repo, which was
# compiler for x86-64 architecture
CC = gcc
INC = -I/usr/local/inc -I/usr/local/include/ejovo/matrix -I/usr/local/include/ejovo
LIB = -lejovo -lm
SRC = qtest.c

compile: $(SRC)
	$(CC) $(SRC) -o q $(LIB) $(INC) -static -static-libgcc