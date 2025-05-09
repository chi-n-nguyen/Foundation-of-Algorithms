CC = clang
CFLAGS = -Wall -Wextra -Wno-newline-eof -Wno-unused-parameter -pedantic -std=c17 
LDFLAGS = -lm

PROGRAM = a1
SOURCE_FILES = a1.c
HEADER_FILES = $(shell find . -type f -name '*.h')
OBJECT_FILES = $(SOURCE_FILES:.c=.o)

.PHONY: all build

all: build

build: $(PROGRAM)

$(PROGRAM): $(OBJECT_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

