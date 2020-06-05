---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: default
---

# microBlinky for the NRF52
## Blinky in two dozen lines of code

Note: You can find all of the code, build commands and a segger flashing utility at [https://github.com/sAlexander/nrf52-minimal-c].

The nrf52 is a great chip: BLE functionality in a small package at a low cost. It's power requirements are low and it's processor is powerful. And the development kit is a great platform for learning.

However, the nrf52 is also quite complex. It's based off the ARM Cortex M4, includes a function-packed but complex softdevice, and has a thick reference guide dedicated to describing its hardware functionality.

A great place to start understanding the nrf52 is the examples provided in the SDK. [SDK 12](http://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk%2Fdita%2Fsdk%2Fnrf5_sdk.html) contains dozens of examples, from blinky (the hello world of microcontrollers), to a BLE heart-rate monitor emulator. These examples show the full range of functionality of the development kit, and can even run on a handful of other boards with the NRF52. Very nice!

The catch: these examples are quite complex. For example, blinky, a sample app that flashes the onboard LEDs, is made up of 21 C files, 100 include folders, three Makefiles, and a couple of linker scripts. The primary Makefile that describes blinky weighs in at 219 lines of code. Ouch!

In the final blinky executable, only a handful of these files are actually needed, and a small portion of the functions. But which ones? How many of those C files are actually used? How much of the 100 lines of linker script is necessary?

## Inspiration from the STM32

In my search for enlightenment, I came across a great resource for the [STM32](https://en.wikipedia.org/wiki/STM32) at [http://pramode.net]. The author put together a very interesting piece on creating a [minimal C program for the STM32](http://pramode.net/fosstronics/minimal-c-program.txt). Given that both the STM32 and the NRF52 share a common heritage (Cortex M series), I was hopeful that some of the ideas could be shared.

(Side note: I would highly recommend reading [all of the author's notes](http://pramode.net/fosstronics) on "fosstronics")

The results of the author's work? 17 lines of easy to understand C code, paired with a 6 line linker script. What a breath of fresh air!

It seemed too good to be true -- I wouldn't believe the results until I was able to reproduce the feat of strength on the NRF52.

## Understanding the code layout

The code layout for the NRF52 matches the layout for the STM32 exactly. For a minimal example, you must provide three things:

Vector Table:
- [4 bytes] Initial stack pointer: This is where the stack pointer will be initialized.
- [4 bytes] Reset vector: This is where the NRF52 will start running code after the device is reset. this is also known as the "start address".

Code:
- Implementation of code to run at the start address

Yep, that's it. Three things. 

When the device starts, it'll (1) read the first four bytes from the device and initialize the stack pointer at the provided address, (2) read the second four bytes from the device and jump to the provided address, and (3) execute the code at that address! This is an over-simplification (the device is busy when it starts up!), but it is enough for creating this minimal example.

You can find a full description of the vector table (the first two items above) in the [Arm Information Center for the Cortex M4](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0553a/BABIFJFG.html).

## Manipulating the GPIO Registers

Once we have our code running, how can we perform simple general purpose input/output (GPIO) tasks, like setting a pin HIGH (light on) and then setting a pin LOW (light off)?

For this, we can consult the nordic product documentation on [GPIO](https://infocenter.nordicsemi.com/topic/com.nordic.infocenter.nrf52832.ps.v1.1/gpio.html?cp=2_2_0_19#concept_zyt_tcb_lr).

The lowest level way of controlling pins is by directly manipulating registers (i.e. memory addresses) on the NRF52. There is a set of registers for configuring each of the pins (input, output, etc), a register for setting pins, and a register for clearing pins.

I should note: Nordic provides a great API for working with these GPIO pins in native C code. For most applications, it's best to use their provided APIs. You can read more about their [GPIO Abstractions](https://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk5.v12.2.0/group__nrf__gpio.html?resultof=%22%6e%72%66%5f%67%70%69%6f%5f%63%66%67%5f%6f%75%74%70%75%74%22%20).


Okay, back to manipulating registers. There we can find a handful of constants that we'll need:

Base Address: **0x50000000**

Offsets (all relative to Base Address):  
Out Set: **0x508**  
Out Clr: **0x50C**  
Pin Config[0]: **0x700**  
Pin Config[1]: **0x704**  
...  
Pin Config[31]: **0x77C**  

With these, we have everything we'll need to blink some lights!

## The Code

**main.c**
```
// Development kit specific values.
#define LED_1 17

// Defined in "GPIO" for the nrf52.
#define CFG_MODE_ON 3
#define SET_PTR (*((volatile unsigned long*) (0x50000000 + 0x508)))
#define CLR_PTR (*((volatile unsigned long*) (0x50000000 + 0x50C)))
#define LED_1_PIN_CNF_PTR  (*((volatile unsigned long*) \
			(0x50000000 + 0x700 + LED_1 * 4)))
        
__asm__(".word 0x20001000");
__asm__(".word main");
int main() {
    LED_1_PIN_CNF_PTR = CFG_MODE_ON;

    unsigned int c = 0;
        
    while(1) {
        SET_PTR = (1 << LED_1);
        for(c = 0; c < 1000000; c++);
        CLR_PTR = (1 << LED_1);
        for(c = 0; c < 1000000; c++);
    }
}
```

**main.lds**
```
SECTIONS {
    . = 0x0;
    .text : {
        *(.text)
    }
 }
```


