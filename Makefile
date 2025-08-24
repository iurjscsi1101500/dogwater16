CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -O2
LDFLAGS := 

OBJS = alu.o control_flow.o memory.o main.o

TARGET = emu

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

