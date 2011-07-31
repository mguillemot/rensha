#ifndef __ASM_INCLUDED__

#define nop()  __asm__ __volatile__("nop")

#define bit_get(p,m) ((p) & (m)) 
#define bit_set(p,m) ((p) |= (m)) 
#define bit_clear(p,m) ((p) &= ~(m)) 
#define bit_flip(p,m) ((p) ^= ~(m)) 
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 

#endif // __ASM_INCLUDED__
