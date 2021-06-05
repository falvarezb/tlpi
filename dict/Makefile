BUILD_DIR = out
LOG_DIR = logs
GCC = gcc
# _POSIX_C_SOURCE=200809 to expose strdup in Linux
CFLAGS = -D_POSIX_C_SOURCE=200809 -g -Wall -Wextra -Wshadow -std=c11 -fprofile-arcs -ftest-coverage

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	VALGRIND = valgrind --tool=memcheck --leak-check=full 
	VALGRIND += --verbose --log-file=${LOG_DIR}/valgrind.log
else
	VALGRIND = 
endif


.PHONY: all runtest clean

all: runtest

runtest: test
	$(VALGRIND) ./${BUILD_DIR}/$^

test: ${BUILD_DIR}/dict.o ${BUILD_DIR}/dict_test.o
	$(GCC) $(CFLAGS) $^ -o ${BUILD_DIR}/$@ -lcmocka


# if an object ﬁle is needed, compile the corresponding .c ﬁle
${BUILD_DIR}/%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

clean:
	rm -f ${LOG_DIR}/* ${BUILD_DIR}/* *.o *.gcov