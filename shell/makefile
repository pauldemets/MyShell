CC = gcc
CFLAGS = -D_XOPEN_SOURCE=700 -std=c11 -Wall -Wextra -pedantic

all: main
main:main.o StringVector.o Shell.o commands.o
main.o: StringVector.h Shell.h commands.h
StringVector.o: StringVector.h
Shell.o: Shell.h
commands.o: commands.h
