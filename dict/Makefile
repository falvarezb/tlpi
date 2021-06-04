BUILD_DIR = out
LOG_DIR = logs
GCC = gcc
CFLAGS = -g -Wall -Wextra -Wshadow -std=c11 -fprofile-arcs -ftest-coverage

#if __linux__
VALGRIND = valgrind --tool=memcheck --leak-check=full 
VALGRIND += --verbose --log-file=${LOG_DIR}/valgrind.log
#else
VALGRIND = 
#endif

test: ${BUILD_DIR}/dict.o ${BUILD_DIR}/dict_test.o
	$(GCC) $(CFLAGS) $^ -o ${BUILD_DIR}/$@ -lcmocka


# if an object ﬁle is needed, compile the corresponding .c ﬁle
${BUILD_DIR}/%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

clean:
	rm -f ${LOG_DIR}/* ${BUILD_DIR}/* *.o *.gcov