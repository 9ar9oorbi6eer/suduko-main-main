CC=gcc
CFLAGS=-Wall -Wextra -g  
LDFLAGS=-pthread         

SRCS=mssv.c threads.c variables.c mssv_utils.c
OBJS=$(SRCS:.c=.o)
DEPS=variables.h threads.h mssv_utils.h

TARGET=mssv

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
