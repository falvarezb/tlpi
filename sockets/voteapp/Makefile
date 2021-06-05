BUILD_DIR = out
LOG_DIR = logs
OUTPUT_DIRS = ${BUILD_DIR} ${LOG_DIR}

CFLAGS = -g -Wall -Wextra -Wshadow -std=c11 -I../../lib -I../ -L../..
SRCS_PROTOCOL := delimiter_framer.c vote_encoding_text.c
SRCS_SV := vote_sv.c $(SRCS_PROTOCOL)
SRCS_CL := vote_cl.c $(SRCS_PROTOCOL)
OBJS_SV := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(SRCS_SV)))
OBJS_CL := $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(SRCS_CL)))
LDLIBS = -ltlpi
EXEC_SV = $(BUILD_DIR)/votesv
EXEC_CL = $(BUILD_DIR)/votecl


UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	VALGRIND = valgrind --tool=memcheck --leak-check=full 
	VALGRIND += --verbose --log-file=${LOG_DIR}/valgrind.log
else
	VALGRIND = 
endif


.PHONY: default runsv runcl clean

default:
	@echo "specify option"

# server config
runsv: $(EXEC_SV)
	$(VALGRIND) ./$^ $(serverport)

installsv: $(EXEC_SV)

$(EXEC_SV): $(OBJS_SV)
	$(LINK.c) $^ -o $@ $(LDLIBS)

$(OBJS_SV): | ${OUTPUT_DIRS}

# client config
runcl: $(EXEC_CL)
	$(VALGRIND) ./$^ $(serverhost) $(serverport) $(candidate) $(inquiry)

installcl: $(EXEC_CL)

$(EXEC_CL): $(OBJS_CL)
	$(LINK.c) $^ -o $@ $(LDLIBS)

$(OBJS_CL): | ${OUTPUT_DIRS}

# if an object ﬁle is needed, compile the corresponding .c ﬁle
${BUILD_DIR}/%.o: %.c
	$(COMPILE.c) $< -o $@


${OUTPUT_DIRS}:
	mkdir $@

clean:
	${RM} ${LOG_DIR}/* ${BUILD_DIR}/* *.o