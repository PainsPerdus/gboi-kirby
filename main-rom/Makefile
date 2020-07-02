# pod = Pineapple of Doom
PROG = pod

export SDCCDIR=/usr  # might not work for everyone
GBDK_2020 = ../gbdk-2020/
CC = $(GBDK_2020)/build/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j
IMG2GB = img2gb
CHUNK_MAKER = cargo run --manifest-path ../podpacker/Cargo.toml 


_OBJ =  $(patsubst %.c,%.o,$(wildcard src/*.c))
# _OBJ =  $(wildcard src/*.o)


all: $(PROG).gb

debug: $(wildcard src/*.c)
	$(CC) -S $^

$(PROG).gb: $(_OBJ)
	$(CC) -o $(PROG).gb $(_OBJ)

%.o: %.c assets chunks
	$(CC) -o $@ -c $<

assets: 
	$(IMG2GB) tileset \
		--output-c-file=src/player.sprites.c \
		--output-header-file=src/player.sprites.h \
		--output-image=src/player.sprites.png \
		--sprite8x16 \
		--name PLAYER_SPRITES \
		./img/piney.png
	$(IMG2GB) tileset \
		--output-c-file=src/chainsaw_lateral.sprites.c \
		--output-header-file=src/chainsaw_lateral.sprites.h \
		--output-image=src/chainsaw_lateral.sprites.png \
		--sprite8x16 \
		--name CHAINSAW_LATERAL_SPRITES \
		./img/chainsaw_lateral.png
	$(IMG2GB) tileset \
		--output-c-file=src/chainsaw_vertical.sprites.c \
		--output-header-file=src/chainsaw_vertical.sprites.h \
		--output-image=src/chainsaw_vertical.sprites.png \
		--sprite8x16 \
		--name CHAINSAW_VERTICAL_SPRITES \
		./chainsaw_vertical.png
	$(IMG2GB) tileset \
		--output-c-file=src/dummy.sprites.c \
		--output-header-file=src/dummy.sprites.h \
		--output-image=src/dummy.sprites.png \
		--sprite8x16 \
		--name DUMMY_SPRITES \
		./img/dummy.png
	$(IMG2GB) tileset \
		--output-c-file=src/tileset.c \
		--output-header-file=src/tileset.h \
		--name TILESET \
		./img/tileset.png

chunks: 
	$(CHUNK_MAKER) ../ogmo/chunks/zero 0 ./src
	$(CHUNK_MAKER) ../ogmo/chunks/one 1 ./src
	$(CHUNK_MAKER) ../ogmo/chunks/two 2 ./src

.PHONY: gbdk-2020
gbdk-2020:
	cd $(GBDK_2020) && make

clean:
	rm -f src/*.rel
	rm -f src/*.lst
	rm -f src/*.sym
	rm -f src/*.asm
	rm -f src/*.o
	rm -f $(PROG).gb
	rm -f $(PROG).ihx
	rm -f $(PROG).map
	rm -f $(PROG).sym
	rm -f $(PROG).noi
	rm -f $(PROG).lk
	rm -f src/*.sprites.*
	rm -f src/tileset.*
	rm -f src/chunks*
