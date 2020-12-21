CC = gcc
INCLUDE_DIR = include
OBJ = $(wildcard src/*.c)
FLAGS = -Werror -Wall -std=c99

app.exe:
	${CC} ${OBJ} -I${INCLUDE_DIR} ${FLAGS}

clean:
	rm *.o *.exe *.out

run: app.exe
	./app.exe