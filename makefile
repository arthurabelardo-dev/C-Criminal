CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -I./include
LDFLAGS =
TARGET  = ccriminal
SRC     = src/main.c src/jogo.c src/historico.c src/utils.c

.PHONY: all clean run

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
