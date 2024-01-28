CFLAGS = -std=c11 -Wall -Werror -Wextra -g -O2

CPPFLAGS = -I ../include/ -DDEBUG

LDFLAGS = -lm

# Target 
TARGET = takuzu

# Source files
SRCS = takuzu.c grid.c

HEADERS = ../include/takuzu.h ../include/grid.h

# Object files
OBJS = $(SRCS:.c=.o)

.PHONY: all clean help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)
	@cp $(TARGET) ..

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Clean temporary files and the binary
clean:
	rm -f $(OBJS) $(TARGET) ../$(TARGET)

# Display help
help:
	@echo "Usage of Makefile:"
	@echo "  make         : Build the takuzu binary"
	@echo "  make clean   : Remove temporary files and the binary"
	@echo "  make help    : Display this help message"
