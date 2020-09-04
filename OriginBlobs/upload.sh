#!/bin/sh

for attempt in {1..10}; do
    echo "Attempt $attempt"
    arduino-cli upload -p /dev/cu.SLAB_USBtoUART --fqbn esp32:esp32:esp32 --input-dir bin/OriginBlobs
    if [ $? -eq 0 ]; then break; fi
done
