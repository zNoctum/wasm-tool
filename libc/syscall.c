
void*
sbrk(unsigned long inc)
{
	unsigned long tmp = sbrk_ptr;
	sbrk_ptr += inc;
	return (void *)tmp;
}
