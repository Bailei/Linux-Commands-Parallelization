.PHONY: clean
CC	:= g++
CFLAGS	:= -std=c++11 -oFast -O3
#CFLAGS	:= -std=c++11 
LDFLAGS := -lboost_regex-mt -pthread
HEADERS	:= ThreadPool.h semaphore.h

OBJS_GREP	:= grep.o
OBJS_COPY	:= copy.o
OBJS_WGET	:= wget.o
OBJS_GZIP	:= gzip.o
OBJS_PARALLEL	:= parallel_do.o
APPS	:= grep copy wget gzip parallel_do

all: $(APPS)

.cc.o:
	$(CC) $(CFLAGS) $< -c -o $@

grep: $(OBJS_GREP) $(HEADERS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

copy: $(OBJS_COPY) $(HEADERS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

wget: $(OBJS_WGET) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

gzip: $(OBJS_GZIP) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

parallel_do: $(OBJS_PARALLEL) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	@rm -f *.o ${APPS}

