// Defined in "Memory Layout" for the nrf52.
#define P0_BASE 0x50000000

// Defined in "GPIO" for the nrf52.
#define SET_OFFSET 0x508
#define CLR_OFFSET 0x50C
#define PIN_CNF_OFFSET 0x700
#define CFG_MODE_ON 3

// Development kit specific values.
#define LED_1 17
#define LED_2 18
#define LED_3 19
#define LED_4 20

#define SET_PTR (*((volatile unsigned long*) (P0_BASE + SET_OFFSET)))
#define CLR_PTR (*((volatile unsigned long*) (P0_BASE + CLR_OFFSET)))
#define LED_1_PIN_CNF_PTR  (*((volatile unsigned long*) (P0_BASE + PIN_CNF_OFFSET + LED_1 * 4)))
        
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
