/*
 * Red LED on PB9, Green on PB8. We will put on Red
 *
 * GPIOB_CRH is initially 0x44444444 (reset state, all pins inputs
 * and floating)
 *
 * We will change GPIOB_CRH to 0x44444414 (PB9 push-pull output)
 *
 * It is possible to make PB9 go high without touching any of the
 * other pins by using the GPIOB_BSRR - we will set the 9th bit of
 * this register to set PB9; ie, we will write (1 << 9) to GPIOB_BSRR
 *
 * GPIOB registers start at 0x40010c00.
 *
 * GPIOB_CRH is at offset 0x4 and GPIOB_BSRR is at offset 0x10
 *
 * The peripheral clock register has to be configured to enable clock
 * to GPIOB. This is done by setting bit 3 of RCC_APB2ENR.
 *
 * The clock registers start at 0x40021000 and RCC_APB2ENR is at 
 * offset 0x018.
 *
 * The STM32F103RB processor has 128Kb flash and 20Kb RAM.
 *
 * 20Kb is 0x5000. RAM starts at 0x20000000. Lets initialize stack
 * pointer to 0x20001000. (4Kb stack)
 *
 */

#define P0_BASE 0x50000000
#define SET_OFFSET 0x508
#define CLR_OFFSET 0x50C
#define PIN_CNF_OFFSET 0x700

#define LED_1          17
#define LED_2          18
#define LED_3          19
#define LED_4          20

#define SET_PTR (*((volatile unsigned long*) (P0_BASE + SET_OFFSET)))
#define CLR_PTR (*((volatile unsigned long*) (P0_BASE + CLR_OFFSET)))
#define LED_1_PIN_CNF_PTR  (*((volatile unsigned long*) (P0_BASE + PIN_CNF_OFFSET + LED_1 * 4)))

        
__asm__(".word 0x20001000");
__asm__(".word main");
int main() {

		volatile unsigned long dir = 1; // Output
		volatile unsigned long input = 1; // Disconnect

		volatile unsigned long cfg = dir
		    | input << 1;
		
		LED_1_PIN_CNF_PTR = cfg;

        unsigned int c = 0;
        
        while(1) {
                SET_PTR = (1 << LED_1);
                for(c = 0; c < 1000000; c++);
                CLR_PTR = (1 << LED_1);
                for(c = 0; c < 1000000; c++);
        }
                
} 
