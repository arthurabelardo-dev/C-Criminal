CC     = gcc
CFLAGS = -Wall -std=c11 -I./include
TARGET = ccriminal
SRC    = src/main.c src/jogo.c src/historico.c src/utils.c src/tui.c
TEST_TARGET = ccriminal_test
TEST_SRC = src/reputacao_test.c src/jogo.c src/historico.c src/utils.c src/tui.c
SESSOES_TARGET = ccriminal_100_sessoes_test
SESSOES_SRC = src/sessoes_100_test.c src/historico.c src/utils.c src/tui.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test:
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)
	$(CC) $(CFLAGS) $(SESSOES_SRC) -o $(SESSOES_TARGET)
	./$(SESSOES_TARGET)

test-100-sessoes:
	$(CC) $(CFLAGS) $(SESSOES_SRC) -o $(SESSOES_TARGET)
	./$(SESSOES_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(SESSOES_TARGET)
