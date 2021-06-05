BUILD_DIR = out
LOG_DIR = logs
GCC = gcc
# _POSIX_C_SOURCE=200809 to expose strdup in Linux
CFLAGS = -D_POSIX_C_SOURCE=200809 -g -Wall -Wextra -Wshadow -std=c11
OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(wildcard *.c)))

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

test: $(OBJS)
	$(GCC) $(CFLAGS) $^ -o ${BUILD_DIR}/$@ -lcmocka

$(OBJS): | ${BUILD_DIR} 

# if an object ﬁle is needed, compile the corresponding .c ﬁle
${BUILD_DIR}/%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@



${BUILD_DIR}:
	mkdir $@

${LOG_DIR}:
	mkdir $@

clean:
	${RM} ${LOG_DIR}/* ${BUILD_DIR}/* *.o