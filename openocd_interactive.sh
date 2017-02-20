#!/bin/bash
BUILD_DIR=./build
DEVICE_BUS_DIR=$(lsusb | grep SEGGER | sed 's/Bus\ \([0-9]*\)\ Device\ \([0-9]*\).*$/\1\/\2/g')
DEVICE_DIR=/dev/bus/usb/${DEVICE_BUS_DIR}

echo $DEVICE_DIR

docker build -t ocd-image --file openocd.dockerfile .
docker run \
	--device=${DEVICE_DIR} \
	-v $(readlink -f $BUILD_DIR):/build \
	-t -i ocd-image bash

# example command:
#    openocd -f /openocd-0.10.0/tcl/board/nordic_nrf52_dk.cfg -c init -c "reset init" -c halt -c "nrf52 mass_erase"
