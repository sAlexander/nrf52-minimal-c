# NRF52 Minimal C
## Blinky in two dozen lines of code

A very simple example for the Nordic nrf52832 or nrf52840 microcontroller. For a more detailed description, see:

https://salexander.github.io/nrf52-minimal-c/

## Building

Dependencies:
-  [GNU Make](https://www.gnu.org/software/make/)
-  [GNU ARM Embedded Toolchain](https://launchpad.net/gcc-arm-embedded)

The logic for building the project is in the [Makefile](https://github.com/sAlexander/nrf52-minimal-c/blob/master/Makefile). With GNU Make and the GNU ARM Embedded Toolchain, you can build the binary with:

```
make
```

## Flashing on the NRF52 Development Kit

If you have [Docker](https://www.docker.com/) installed, you can build the example and flash to an NRF52 Development Kit board with:

```
./build_and_flash.sh
```


