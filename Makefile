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
	rm -f *.txt; ./$(BIN)/ftp ftp://[$(user):$(password)@]$(host)/$(url)

.PHONY: run_test
run_test: $(BIN)/ftp
	rm -f *.txt; ./$(BIN)/ftp ftp://[anonymous:anonymous@]ftp.up.pt/pub/kodi/timestamp.txt

.PHONY: run_lab
run_lab: $(BIN)/ftp
	rm -f *.txt; ./$(BIN)/ftp ftp://[anonymous:anonymous@]netlab1.fe.up.pt/pub/kodi/timestamp.txt

.PHONY: clean
clean:
	rm -f $(BIN)/ftp *.txt