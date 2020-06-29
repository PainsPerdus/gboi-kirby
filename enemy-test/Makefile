# pod = Pineapple of Doom
PROG = pod_enemy

export SDCCDIR=~/sdcc  # might differ
GBDK_2020 = ../gbdk-2020/
CC = $(GBDK_2020)/build/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j
IMG2GB = img2gb

_OBJ = $(patsubst %.c,%.o,$(wildcard src/*.c))

all: build

build: $(PROG).gb

debug: $(wildcard src/*.c)
	$(CC) -S $^

$(PROG).gb: $(_OBJ)
	$(CC) -o $(PROG).gb $^

%.o: %.c
	$(CC) -o $@ -c $<

assets:
	$(IMG2GB) tileset \
		--output-c-file=src/dummy-tileset.c \
		--output-header-file=src/dummy-tileset.h \
		--output-image=src/dummy-tileset.png \
		--sprite8x16 \
		--name DUMMY_SPRITES \
		./dummy.png
	$(IMG2GB) tileset \
		--output-c-file=src/tileset.c \
		--output-header-file=src/tileset.h \
		--name TILESET \
		./tileset.png
		
.PHONY: gbdk-2020
gbdk-2020:
	cd $(GBDK_2020) && make

clean:
	rm -rf src/*.rel
	rm -rf src/*.lst
	rm -rf src/*.sym
	rm -rf src/*.asm
	rm -rf src/*.o
	rm -rf $(PROG).gb
	rm -rf $(PROG).ihx
	rm -rf $(PROG).map
	rm -rf $(PROG).sym
	rm -rf $(PROG).noi
	rm -rf $(PROG).lk
	rm src/dummy-tileset.*