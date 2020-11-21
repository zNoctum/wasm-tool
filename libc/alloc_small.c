void*
malloc(size_t size)
{
	unsigned int tmp = sbrk_ptr;
	*((void *)sbrk_ptr) = size;
	sbrk_ptr += size + sizeof(unsigned int);
	return ((void *)(tmp + sizeof(int)));
}

void
free(void *block)
{  }

void*
realloc(void *block, size_t size)
{
	void *new_block = malloc(size);
	memcpy(new_block, block, *(int *)(((unsigned int)block) - sizeof(int)));
	return new_block;
}

void*
calloc(size_t nitems, size_t size)
{
	return malloc(nitems * size);
}
