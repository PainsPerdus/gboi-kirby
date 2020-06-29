# pod = Pineapple of Doom
PROG = floorgen

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
