CC     = gcc
CFLAGS = -Wall -std=c11
TARGET = ccriminal
SRC    = main.c jogo.c historico.c utils.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)