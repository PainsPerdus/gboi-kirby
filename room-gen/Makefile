CC = ../gbdk-2020/build/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j

CHUNK_MAKER = cargo run --manifest-path ../ogmo/ogmo-parser/Cargo.toml 

PROG = room-gen

IMG2GB = img2gb
OBJ =  $(patsubst %.c,%.o,$(wildcard src/*.c))
BINS = room-gen.gb



all: $(BINS)

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

$(PROG).gb:	$(OBJ)
	$(CC) -o $(PROG).gb $^

assets: tileset.png
	img2gb tileset \
    	--output-c-file=src/tileset.c \
    	--output-header-file=src/tileset.h \
    	--deduplicate \
    	./tileset.png

chunks: 
	$(CHUNK_MAKER) ../ogmo/chunks/zero 0 ./src/ 
	$(CHUNK_MAKER) ../ogmo/chunks/zero 1 ./src/
	$(CHUNK_MAKER) ../ogmo/chunks/zero 2 ./src/

clean:
	rm -rf src/*.o	
	rm -rf src/*.lst
	rm -rf src/*.sym
	rm -rf $(PROG).map
	rm -rf $(PROG).sym