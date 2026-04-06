CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = server
OBJS = main.o network.o parser.o file_io.o thread_pool.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
