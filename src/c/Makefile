# Compiler and flags
CC = gcc
CFLAGS = -Wall -g  

# List of source files
SRC = main.c
# Generate a list of object files from source files
OBJ = $(SRC:.c=.o)

# Target executable
TARGET = ims-ca

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm
	rm -f *.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)