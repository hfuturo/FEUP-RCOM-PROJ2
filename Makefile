CC = gcc
CFLAGS = -Wall

SRC = src/
BIN = bin/
INCLUDE = include/

.PHONY: all
all: $(BIN)/download

$(BIN)/download: $(SRC)/*.c
			$(CC) $(CFLAGS) -o $@ $^ -I$(INCLUDE)

.PHONY: run
run: $(BIN)/download
	rm -f *.txt *.mp4 *.png *.jpg; ./$(BIN)/download ftp://$(user):$(password)@$(host)/$(url)

.PHONY: run_teste
run_teste: $(BIN)/download
	rm -f timestamp.txt; ./$(BIN)/download ftp://anonymous:anonymous@ftp.up.pt/pub/kodi/timestamp.txt

.PHONY: run_teste1
run_teste1: $(BIN)/download
	rm -f timestamp.txt; ./$(BIN)/download ftp://ftp.up.pt/pub/kodi/timestamp.txt

.PHONY: run_teste2
run_teste2: $(BIN)/download
	rm -f pub.txt; ./$(BIN)/download ftp://anonymous:anonymous@netlab1.fe.up.pt/pub.txt

.PHONY: run_teste3
run_teste3: $(BIN)/download
	rm -f pub.txt; ./$(BIN)/download ftp://netlab1.fe.up.pt/pub.txt

.PHONY: run_teste4
run_teste4: $(BIN)/download
	rm -f pipe.txt; ./$(BIN)/download ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt

.PHONY: run_teste5
run_teste5: $(BIN)/download
	rm -f crab.mp4; ./$(BIN)/download ftp://rcom:rcom@netlab1.fe.up.pt/files/crab.mp4

.PHONY: run_teste6
run_teste6: $(BIN)/download
	rm -f pic1.jpg; ./$(BIN)/download ftp://rcom:rcom@netlab1.fe.up.pt/files/pic1.jpg

.PHONY: run_teste7
run_teste7: $(BIN)/download
	rm -f pic2.png; ./$(BIN)/download ftp://rcom:rcom@netlab1.fe.up.pt/files/pic2.png

.PHONY: clean
clean:
	rm -f $(BIN)/download *.txt *.jpg *.png *.mp4