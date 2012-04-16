CC = gcc
CFLAGS = -g3 -o0 -Wall -I./include
LDLIBS = -lreadline
SRC_DIR = src
BUILD_DIR = build
OBJS = \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/error.o \
	$(BUILD_DIR)/context.o \
	$(BUILD_DIR)/memory.o \
	$(BUILD_DIR)/read.o \
	$(BUILD_DIR)/token.o \
	$(BUILD_DIR)/parser.o
TAR = $(BUILD_DIR)/lispo

all : $(TAR)

$(TAR) : $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/main.o : $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/error.o : $(SRC_DIR)/error.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/context.o : $(SRC_DIR)/context.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/memory.o : $(SRC_DIR)/memory.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/read.o : $(SRC_DIR)/read.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/token.o : $(SRC_DIR)/token.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/parser.o : $(SRC_DIR)/parser.c
	$(CC) $(CFLAGS) -c $^ -o $@

./PHONY : clean

clean :
	rm $(OBJS) $(TAR)
