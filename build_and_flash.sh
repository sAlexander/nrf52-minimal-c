#!/bin/bash

BUILD_DIR=./build

DEVICE_BUS_DIR=$(lsusb | grep SEGGER | sed 's/Bus\ \([0-9]*\)\ Device\ \([0-9]*\).*$/\1\/\2/g')
DEVICE_DIR=/dev/bus/usb/${DEVICE_BUS_DIR}

make -f Makefile all

docker build -t segger-image --file segger.dockerfile .
docker run -v $(readlink -f $BUILD_DIR):/build \
    --device=${DEVICE_DIR} segger-image \
    /bin/bash -c "
    nrfjprog -i && \
    nrfjprog --family nRF52 -e && \
	nrfjprog --family nRF52 --program /build/main.hex && \
	nrfjprog --family nRF52 -r"
