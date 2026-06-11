CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -fPIC -I include
LDFLAGS = -Wl,-rpath,'$$ORIGIN'
SANITIZER_FLAGS = -fsanitize=address,undefined -g
ANALYZER = clang-tidy
SOURCES = src/histogram.c
OBJECTS = $(SOURCES:.c=.o)
TARGET_LIB = libhistogram.so
TEST_SRC = test/test.c
TEST_BIN = test_histogram

.PHONY: all clean test python_test docs analyze sanitize

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJECTS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TARGET_LIB) $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -L. -lhistogram -o $@

test: $(TEST_BIN)
	LD_LIBRARY_PATH=. ./$(TEST_BIN)

python_test: $(TARGET_LIB)
	cd test && python3 test.py

sanitize: clean
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -g -c $(SOURCES) -o src/histogram.o
	$(CC) $(SANITIZER_FLAGS) -shared -o $(TARGET_LIB) src/histogram.o $(LDFLAGS)
	$(CC) $(CFLAGS) $(SANITIZER_FLAGS) -g test/test.c -L. -lhistogram -o $(TEST_BIN)
	LD_LIBRARY_PATH=. ./$(TEST_BIN)

analyze:
	mkdir -p reports
	$(ANALYZER) src/histogram.c -- $(CFLAGS) 2>&1 | tee reports/clang-tidy.log

docs:
	doxygen Doxyfile

clean:
	rm -f src/*.o $(TARGET_LIB) $(TEST_BIN)
	rm -rf reports
	rm -rf docs/
