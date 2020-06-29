#include "stm32f10x.h"

/* Bit Band Macros used to calculate the alias address at run time */
#define ADDRESS(x)     (*((volatile unsigned long *)(x)))
#define BitBand(x, b)  ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((b) << 2))

#define PERIPH_BASE    ((uint32_t)0x40000000)
#define AHBPERIPH_BASE (PERIPH_BASE + 0x20000)
#define GPIOB_BASE     (AHBPERIPH_BASE + 0x0400)    /* 0x40020400 */

/* Calculate bit band alias address 32MB above PERIPH_BASE. Bit band region is 1MB above PERIPH_BASE. 32MB is 33,554,432 = 0x2000000 so
   bit_word_offset = (byte_offset × 32) + (bit_number × 4) page 41 m3 trm
   bit_word_addr = bit_band_base + bit_word_offset
   0x42020400 plus offset to GPIOB_ODR (0x14) plus bit 6 0x6 = 0x42020400 + 0x3E8 + 0x18 */

/* Pointer using bit band alias address calculated at compile time */
#define PortB_Bit8     (*((volatile unsigned long *) 0x422181A0))

volatile unsigned long * bitAddress;

void delay(void){
    uint32_t Delay;
    for(Delay = 0; Delay < 0x100000; Delay++){
        ;
    }
}

int main(void){
    while(1){
        /* 0b100000000 = 0x100 */
        GPIOB->ODR |= 0x00000100;                   /* Set and Clear the bit using port wide AND and OR instructions - Slowest method */
        delay();
        GPIOB->ODR &= !0x00000100;	  
        delay();
        bitAddress = & BitBand(&GPIOB->ODR, 8);			/* Use a run time macro to calculate the bit band alias address - 2nd Fastest method */
        *bitAddress = 1;														/* Set and Clear the bit */ 
        delay();
        *bitAddress = 0;
        delay();
        PortB_Bit8 = 1;															/* Set and Clear the bit using a pointer calculated at compile time - Fastest method */
        delay();
        PortB_Bit8  = 0;  
        delay();
    }
}
