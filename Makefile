CC:=gcc
CFLAGS:=-Wall -std=gnu11 -Werror -I src 
LD_FLAGS:=-static -llibcmock -v
DEBUG_FLAGS:=-g -O0
RELEASE_FLAGS:=-O3
SRC:=$(shell find src -name '*.c')
TESTS:=$(shell find tests -name '*.c')
OBJ:=$(SRC:.cpp=.o)
TEST_OBJ:=$(TESTS:.cpp=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

jsonlog: $(OBJ)
	@$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $@ $^

.PHONY: debug release test clean

clean:
	@rm -f debug test jsonlog

debug: $(OBJ)
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $@ $^

release: jsonlog

test: $(TEST_OBJ) $(OBJ)
	@date
	@$(CC) $(LD_FLAGS) -o $@ $^
	@./test
