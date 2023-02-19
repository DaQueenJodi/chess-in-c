SRCS	:= chess.c board.c util.c pieces.c
CFLAGS := -Wall -Wpedantic -Wextra -ggdb
BIN	:= chess
CC	:= clang

all: $(BIN)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)

run:
	./$(BIN)

clean:
	rm -f $(BIN)

debug:
	gdb $(BIN)
