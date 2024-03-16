CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lpng -ljpeg -lm

all: image-fmt-convert

image-fmt-convert: image-fmt-convert.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f image-fmt-convert image-fmt-convert.o
