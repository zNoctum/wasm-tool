void*
memcpy(void *dest, const void *src, unsigned long n)
{
	int i;
	for(i = 0; i < n; i++)
		((char *)dest)[i] = ((char *)src)[i];
	return dest;
}

void*
memset(void *ptr, int val, unsigned long n)
{
	int i;
	for(i = 0; i < n; i++)
		((int *)ptr)[i] = val;
	return ptr;
}
