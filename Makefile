
#Sets compiler to GCC
CC = gcc


#Enables warning signs, both normal and extra
CFLAGS = -Wall -Wextra -g 

# pkg-config shows where the SDL3 things are
LDFLAGS = $(shell pkg-config --libs sdl3) -lm

# output path
TARGET = ./out/stealth_game

all: $(TARGET)


$(TARGET): src/main.c
	mkdir -p out	# Create output dir if not there
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) # The actual command

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf out

.PHONY:
	all run clean
