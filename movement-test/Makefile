# pod = Pineapple of Doom
PROG = pod

export SDCCDIR=/usr  # might not work for everyone
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
		--output-c-file=src/player.sprites.c \
		--output-header-file=src/player.sprites.h \
		--output-image=src/player.sprites.png \
		--sprite8x16 \
		--name PLAYER_SPRITES \
		./piney.png
	$(IMG2GB) tileset \
		--output-c-file=src/chainsaw.sprites.c \
		--output-header-file=src/chainsaw.sprites.h \
		--output-image=src/chainsaw.sprites.png \
		--sprite8x16 \
		--name CHAINSAW_SPRITES \
		./chainsaw.png
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
	rm src/player.sprites.*
	rm src/tileset.*
