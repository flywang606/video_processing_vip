CC=gcc
INCLUDE=
TARGET=gen_bin
FLAG=-D HDL_SIM
INCLUDE= -I ./include

all:
	$(CC) -o $(TARGET) ./src/*.c ./common/*.c $(FLAG) $(INCLUDE)
clean:
	rm -rf $(TARGET)