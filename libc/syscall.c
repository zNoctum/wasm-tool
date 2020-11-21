extern unsigned char __heap_base;
unsigned long sbrk_ptr = (unsigned long)&__heap_base;

void*
sbrk(unsigned long inc)
{
	unsigned long tmp = sbrk_ptr;
	sbrk_ptr += inc;
	return (void *)tmp;
}
