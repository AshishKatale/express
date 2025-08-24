CFLAGS = -Wall -Wextra -O3
LIBS = -I ./src -lm -lreadline
SRC = $(wildcard src/*.c)

main: $(SRC)
	gcc -o build/express $(CFLAGS) $(LIBS) $(SRC)

debug: $(SRC)
	gcc -o build/express-dbg -ggdb $(LIBS) $(SRC)

.PHONY: run clean

run:
	make && ./build/express '1 - 3 * -( 1 + ( 1 + -3))'

dbg:
	make debug && gdb --args ./build/express-dbg '1 - 3 * -( 1 + ( 1 + -3))'

clean:
	rm ./build/*
