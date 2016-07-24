PIXEL_COUNT=16

all: build

monitor:
	pio serialports monitor -b 115200 --raw | ts

upload: build
	pio run -t upload

build: src/table.c
	pio run

src/table.c: table-calc Makefile
	./table-calc ${PIXEL_COUNT} > $@

table-calc: table-calc.o
