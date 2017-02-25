// Development kit specific values.
#define LED_1 17


// Defined in "GPIO" for the nrf52.
#define CFG_MODE_ON 3
#define SET_PTR (*((volatile unsigned long*) (0x50000000 + 0x508)))
#define CLR_PTR (*((volatile unsigned long*) (0x50000000 + 0x50C)))
#define LED_1_PIN_CNF_PTR  (*((volatile unsigned long*) (0x50000000 + 0x700 + LED_1 * 4)))
        
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
