BUILD = build

all: $(BUILD)/main.bin
	

$(BUILD)/main.o : main.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb  -c main.c

$(BUILD)/a.out : $(BUILD)/main.o main.lds
	arm-none-eabi-ld -T main.lds main.o

$(BUILD)/main.bin : $(BUILD)/a.out
	arm-none-eabi-objcopy -j .text a.out main.bin -O binary
