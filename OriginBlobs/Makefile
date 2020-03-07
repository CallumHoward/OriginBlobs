
SRC=$(wildcard *.cpp *.hpp *.ino)
OUT=OriginBlobs

.PHONY: all clean

all: $(SRC)
	@arduino-cli compile \
		--fqbn esp32:esp32:esp32:PartitionScheme=min_spiffs \
		--output bin/$(OUT) \
		--warnings all \
		"$(PWD)" \
		2>&1 | sed -E 's%(/.+)+/sketch/%%' 1>&2

clean:
	rm -rf {bin,cache}/* *.gch
