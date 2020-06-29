# How-to-Experiment-with-Bit-Banding-
Tinkering with Bit-Banding and Profiling

Bit-Band and Execution Profiling Experiments	June 29, 2020

From my previous tutorial on Computer Organization and Memory-Mapped Areas, we 
dwelt on bit-band regions and bit-band alias. The bit-band region is that part 
of memory space where we have byte addressable memory addresses. They were 
assigned and must be adhered to by the ARM standards body. Silicon vendors must
be compliant to the specifications and usually they differentiate their products
by offering differentiated peripherals and custom example code. The designated 
memory regions allow for 8 classifications for data and instructions. For
example they allocate 0.5GB for the smallest of the first region from 0x00000000
to 0x20000000. You will notice that is a lot of memory, and also for the other
regions, hence, a lot of leeway for differentiation. Code is reserved in the 
first region and if a silicon vendor designs a part as having 512K flash meaning
524,288 Bytes or 0x80000. 0x80000 out of 0.5GB(500,000,000 = 0x1DCD6500) used 
means plenty of space allocation.

Anyways, going back to bit-band regions, 1MB is reserved above 0x20000000 and
0x40000000 for bit-band. Each bit in real memory is word aliased by about 32MB
above the base address.

To calculate the alias address so we can create a pointer and cast it to memory,
the given formula is:

   bit_word_offset = (byte_offset × 32) + (bit_number × 4) page 41 m3 trm
   
   bit_word_addr = bit_band_base + bit_word_offset

I read starting from page 27 of the ARM® Cortex®-M3 Processor Revision: r2p1
Technical Reference Manual and please refer to documentations for more guidance.

Include the necessary header and c files to your application code and experiment.

Try to use the simulator when debugging. I used the ARM Compiler v6.14, set dialog
dll to DARMSTM.DLL, set parameter to -pSTM32L152RB, and execute profiling. 

Given the timing results, assembly generated codes, bit-banding is much faster than
other methods. Compare 4.016 ms execution time for GPIOB->ODR |= 0x00000100; versus
1.606 ms for bit-banded PortB_Bit8 = 1; instruction.

Following is the assembly generated code from the disassembly listing. Under the
hood, comparisons.

Happy coding!

    28: int main(void){ 
    
    29:     while(1){ 
    
    30:         /* 0b100000000 = 0x100 */ 
    
0x08000318 4807      LDR      r0,[pc,#28]  ; @0x08000338

0x0800031A 4908      LDR      r1,[pc,#32]  ; @0x0800033C

0x0800031C 4A08      LDR      r2,[pc,#32]  ; @0x08000340

0x0800031E 2300      MOVS     r3,#0x00

0x08000320 2401      MOVS     r4,#0x01

    31:         GPIOB->ODR |= 0x00000100;                   /* Set and Clear the bit using port wide AND and OR instructions - Slowest method */ 
    
    32:         delay(); 
    
0x08000322 6805      LDR      r5,[r0,#0x00]

0x08000324 F4457580  ORR      r5,r5,#0x100

0x08000328 6005      STR      r5,[r0,#0x00]

    33:         GPIOB->ODR &= !0x00000100;     
    
    34:         delay(); 
    
    35:         bitAddress = & BitBand(&GPIOB->ODR, 8);   /* Use a run time macro to calculate the bit band alias address - 2nd Fastest method */ 
    
0x0800032A 6805      LDR      r5,[r0,#0x00]

0x0800032C 6003      STR      r3,[r0,#0x00]
    36:         *bitAddress = 1;                                                                                                                /* Set and Clear the bit */ 
    
    37:         delay(); 
    
0x0800032E 6014      STR      r4,[r2,#0x00]

    38:         *bitAddress = 0; 
    
    39:         delay(); 
    
0x08000330 6013      STR      r3,[r2,#0x00]

    40:         PortB_Bit8 = 1;               /* Set and Clear the bit using a pointer calculated at compile time - Fastest method */ 
    
    41:         delay(); 
    
0x08000332 600C      STR      r4,[r1,#0x00]

    42:         PortB_Bit8  = 0;   
    
0x08000334 600B      STR      r3,[r1,#0x00]

    29:     while(1){ 
    
0x08000336 E7F4      B        0x08000322

0x08000338 040C      DCW      0x040C

0x0800033A 4002      DCW      0x4002

Happy coding once again!!!



