# Makefile variables
CC = gcc
CFLAGS = -fPIC -shared
INCLUDE = -I/usr/local/cuda-12.6/targets/x86_64-linux/include/
LDFLAGS = -ldl -lrt -lcuda -lcudart
TARGET = ptuvm.so

# Source files
SRC = cuda_intercept.c util.c

# Default target
all: $(TARGET)

# Build and move the concurrent .so file to lib directory
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -DENABLE_CONCURRENT -o $(TARGET) $(SRC) $(INCLUDE) $(LDFLAGS)

# Clean up all generated files
clean:
	rm -rf $(TARGET)