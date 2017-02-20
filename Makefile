BUILD = build

all: bin hex

bin: $(BUILD)/main.bin

hex: $(BUILD)/main.hex
	
$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/main.o : main.c
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb  -c main.c
	mv main.o $(BUILD)

$(BUILD)/a.out : $(BUILD)/main.o main.lds
	arm-none-eabi-ld -T main.lds $(BUILD)/main.o
	mv a.out $(BUILD)

$(BUILD)/main.bin : $(BUILD)/a.out
	arm-none-eabi-objcopy -j .text $(BUILD)/a.out $(BUILD)/main.bin -O binary

$(BUILD)/main.hex : $(BUILD)/a.out
	arm-none-eabi-objcopy -j .text $(BUILD)/a.out $(BUILD)/main.hex -O ihex

