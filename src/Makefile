.PHONY: clean
CC	:= g++
CFLAGS	:= -std=c++0x
HEADERS	:= ThreadPool.h

OBJS_GREP	:= grep.o
OBJS_COPY	:= copy.o
OBJS_FIND	:= find.o
APPS	:= grep copy find

all: $(APPS)

.cc.o:
	$(CC) $(CFLAGS) $< -c -o $@

grep: $(OBJS_GREP) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

copy: $(OBJS_COPY) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

find: $(OBJS_FIND) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	@rm -f *.o ${APPS}

