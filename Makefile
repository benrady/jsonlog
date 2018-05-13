CC:=gcc
CFLAGS:=-Wall -std=gnu11 -Werror -I src
DEBUG_FLAGS:=-g -O0
RELEASE_FLAGS:=-O3
TEST_FLAGS:=-l cmocka -Wl,--wrap=socket,--wrap=perror,--wrap=exit
SRC:=$(shell find src/jsonlog -name '*.c')
MAIN_SRC:=src/main.c
TEST_SRC:=$(shell find tests -name '*.c')
OBJ:=$(SRC:.c=.o)
MAIN_OBJ:=$(MAIN_SRC:.c=.o)
TESTS:=$(patsubst tests/jsonlog/%.c, build/tests/%, $(TEST_SRC))
TEST_RESULTS:=$(patsubst tests/jsonlog/%.c, build/results/%.txt, $(TEST_SRC))

# Don't delete the test binaries if any tests fail
.PRECIOUS: build/tests/% build/results/%

help:
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

# Debug/Release flags not used here :-(
%.o: src/jsonlog/%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

build:
	@mkdir -p build/tests
	@mkdir -p build/results

build/tests/%: tests/jsonlog/%.c $(OBJ) | build
	@$(CC) -o $@ $< $(OBJ) $(CFLAGS) $(DEBUG_FLAGS) $(TEST_FLAGS) 

build/results/%.txt: build/tests/% | build # Runs tests in parallel when using -j
	-@$^ > $@ 2>&1

jsonlog: $(OBJ) $(MAIN_OBJ) | build
	@$(CC) -o build/$@ $^ $(CFLAGS) $(RELEASE_FLAGS)

jsonlog_dbg: $(OBJ) $(MAIN_OBJ) | build
	@$(CC) -o build/$@ $^ $(CFLAGS) $(DEBUG_FLAGS)

.PHONY: release test clean help

ct:	## Compile and run tests continuously with entr
	find Makefile src tests | entr -c make --no-print-directory -j test

clean:  ## Remove build artifacts
	@rm -rf build

debug: jsonlog_dbg ## Build a debug binary

release: jsonlog ## Build a release binary

run: debug ## Compile and run a debug build
	@build/jsonlog_dbg

test: $(TEST_RESULTS) ## Compile and run the tests
	@echo
	@date
	@cat $(TEST_RESULTS)
