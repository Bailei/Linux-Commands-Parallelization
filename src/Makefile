.PHONY: clean
CC			:= g++
CFLAGS	:= -std=c++0x
HEADERS	:= ThreadPool.h

OBJS_GREP := grep.o
APPS			:= grep

all: $(APPS)

.cc.o:
	$(CC) $(CFLAGS) $< -c -o $@

grep: $(OBJS_GREP) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


clean:
	@rm -f *.o ${APPS}

