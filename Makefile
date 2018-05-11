CC:=gcc
CFLAGS:=-Wall -std=gnu11 -Werror -I src
LD_FLAGS:=-l cmocka
DEBUG_FLAGS:=-g -O0
RELEASE_FLAGS:=-O3
SRC:=$(shell find src/jsonlog -name '*.c')
MAIN_SRC:=src/main.c
TESTS:=$(shell find tests -name '*.c')
OBJ:=$(SRC:.c=.o)
MAIN_OBJ=$(MAIN_SRC:.c=.o)
TEST_OBJ:=$(TESTS:.c=.o)

help:
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

# Debug/Release flags not used here :-(
%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

build:
	@mkdir -p build

jsonlog: $(OBJ) $(MAIN_OBJ) | build
	@$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o build/$@ $^

jsonlog_dbg: $(OBJ) $(MAIN_OBJ) | build
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o build/$@ $^

.PHONY: release test clean help

ct:	## Compile and run tests continuously with entr
	find Makefile src tests | entr -c make --no-print-directory test

clean:  ## Remove build artifacts
	@rm -rf build
	@find tests src -name *.o | xargs rm

debug: jsonlog_dbg ## Build a release binary

release: jsonlog ## Build a release binary

test: $(TEST_OBJ) $(OBJ) | build ## Compile and run the tests
	@date
	@$(CC) -o build/$@ $^ $(LD_FLAGS) 
	@./build/test

