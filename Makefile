CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror
OUTDIR = build
OUTFILE = $(OUTDIR)/cu_array
SRC = tests/test_cu_array.c

all: $(OUTFILE)

$(OUTFILE): $(SRC)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $(OUTFILE) $(SRC)

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean