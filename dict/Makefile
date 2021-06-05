BUILD_DIR = out
LOG_DIR = logs
OUTPUT_DIRS = ${BUILD_DIR} ${LOG_DIR}

# _POSIX_C_SOURCE=200809 to expose strdup in Linux
CFLAGS = -D_POSIX_C_SOURCE=200809 -g -Wall -Wextra -Wshadow -std=c11
OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(wildcard *.c)))
LDLIBS = -lcmocka
EXEC = $(BUILD_DIR)/test


UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	VALGRIND = valgrind --tool=memcheck --leak-check=full 
	VALGRIND += --verbose --log-file=${LOG_DIR}/valgrind.log
else
	VALGRIND = 
endif


.PHONY: all runtest clean

all: runtest

runtest: $(EXEC)
	$(VALGRIND) ./$^

$(EXEC): $(OBJS)
	$(LINK.c) $^ -o $@ $(LDLIBS)

$(OBJS): | ${OUTPUT_DIRS}

# if an object ﬁle is needed, compile the corresponding .c ﬁle
${BUILD_DIR}/%.o: %.c
	$(COMPILE.c) $< -o $@


${OUTPUT_DIRS}:
	mkdir $@

clean:
	${RM} ${LOG_DIR}/* ${BUILD_DIR}/* *.o