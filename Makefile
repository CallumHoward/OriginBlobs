all:
	arduino-cli compile --fqbn esp32:esp32:esp32 "$(PWD)" 2>&1 | sed -E 's%(/.+)+/sketch/%%' 1>&2
	mv *.{elf,bin} bin/
