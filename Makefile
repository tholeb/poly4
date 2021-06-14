CC     := gcc
CFLAGS := -Iinclude -O2 -pedantic -std=c17 -Wall -Wextra

.PHONY: all clean

all: bin/client bin/server

bin/client: obj/client.o obj/grille.o
	@mkdir -p $(@D)

	$(CC) $^ -o $@

bin/server: obj/server.o obj/grille.o
	@mkdir -p $(@D)

	$(CC) $^ -o $@

obj/%.o: src/%.c
	@mkdir -p $(@D)

	$(CC) -c $^ $(CFLAGS) -o $@

clean:
	rm -f bin/* obj/*
