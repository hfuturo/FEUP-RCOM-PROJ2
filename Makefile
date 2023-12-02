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
	rm -f *.txt *.mp4 *.png *.jpg; ./$(BIN)/ftp ftp://[$(user):$(password)@]$(host)/$(url)

.PHONY: run_teste
run_teste: $(BIN)/ftp
	rm -f timestamp.txt; ./$(BIN)/ftp ftp://[anonymous:anonymous@]ftp.up.pt/pub/kodi/timestamp.txt

.PHONY: run_teste1
run_teste1: $(BIN)/ftp
	rm -f pub.txt; ./$(BIN)/ftp ftp://[anonymous:anonymous@]netlab1.fe.up.pt/pub.txt

.PHONY: run_teste2
run_teste2: $(BIN)/ftp
	rm -f pipe.txt; ./$(BIN)/ftp ftp://[rcom:rcom@]netlab1.fe.up.pt/pipe.txt

.PHONY: run_teste3
run_teste3: $(BIN)/ftp
	rm -f crab.mp4; ./$(BIN)/ftp ftp://[rcom:rcom@]netlab1.fe.up.pt/files/crab.mp4

.PHONY: run_teste4
run_teste4: $(BIN)/ftp
	rm -f pic1.jpg; ./$(BIN)/ftp ftp://[rcom:rcom@]netlab1.fe.up.pt/files/pic1.jpg

.PHONY: run_teste5
run_teste5: $(BIN)/ftp
	rm -f pic2.png; ./$(BIN)/ftp ftp://[rcom:rcom@]netlab1.fe.up.pt/files/pic2.png

.PHONY: clean
clean:
	rm -f $(BIN)/ftp *.txt *.jpg *.png *.mp4