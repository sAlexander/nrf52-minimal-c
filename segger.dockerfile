FROM debian:jessie

RUN apt-get update
RUN apt-get -y dist-upgrade
RUN apt-get -y install curl

ADD vendor /vendor

# Setup nrfjprog
RUN tar -xvf /vendor/nordic/nrfjprog/nRF5x-Command-Line-Tools_9_3_1_Linux-x86_64.tar && \
    ln -s /nrfjprog/nrfjprog /usr/bin/nrfjprog

RUN dpkg -i /vendor/segger/jlink/JLink_Linux_V612j_x86_64.deb
