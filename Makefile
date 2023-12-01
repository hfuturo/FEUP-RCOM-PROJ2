CC = gcc
CFLAGS = -Wall

SRC = src/
BIN = bin/
INCLUDE = include/

.PHONY: all
all: $(BIN)/ftp

$(BIN)/ftp: $(SRC)/*.c
			$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDE)

.PHONY: run
run: $(BIN)/ftp
	./$(BIN)/ftp ftp://[$(user):$(password)@]$(host)/$(url)

.PHONY: run_test
run_test: $(BIN)/ftp
	./$(BIN)/ftp ftp://[anonymous:anonymous@]ftp.up.pt/pub/kodi/timestamp.txt

.PHONY: run_test2
run_test2: $(BIN)/ftp
	./$(BIN)/ftp ftp://[user:password@]ftp.up.pt/file.txt

.PHONY: clean
clean:
	rm -f $(BIN)/ftp