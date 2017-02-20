FROM debian:jessie

RUN apt-get update
RUN apt-get -y dist-upgrade

RUN apt-get -y install bzip2 build-essential libusb-1.0-0-dev libusb-1.0-0 libusb-dev libhidapi-dev libtool pkg-config libftdi-dev

COPY vendor /vendor

RUN tar xvjf /vendor/OpenOCD/openocd-0.10.0.tar.bz2 && \
		cd openocd-0.10.0 && \
		./configure && \
		make && \
		make install
