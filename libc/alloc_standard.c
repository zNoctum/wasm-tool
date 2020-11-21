void*
malloc(size_t in_size)
{
	size_t size = in_size + 8 - (in_size % 8);
	void *block;
	union header *header;
	if (!size)
		return NULL;
	header = head;
	while(header) {
		if(header->s.is_free && header->s.size >= size) {
			memset((void *)(header + 1), 0x00, header->s.size);
			return (void *)(header + 1);
		}
		else if(header->s.is_free && header->s.next->s.is_free &&
				(header->s.size + sizeof(union header) + header->s.next->s.size >= size)) {
			size_t size = header->s.size + sizeof(union header) + header->s.next->s.size;
			header->s.next = header->s.next->s.next;
			memset((void *)(header + 1), 0x00, size);
			return (void *)(header + 1);

		}
		header = header->s.next;
	}
	block = (void *)sbrk_ptr;
	sbrk_ptr += sizeof(union header) + size;

	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
		head = header;
	if (tail)
		tail->s.next = header;
	tail = header;

	memset((void *)(header + 1), 0x00, header->s.size);
	return (void *)(header + 1);
}

void
free(void *block)
{
	union header *header, *tmp;

	if(!block)
		return;
	header = (union header*)block - 1;

	if((unsigned int)block + header->s.size == sbrk_ptr) {
		if(head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;
			while(tmp) {
				if(tmp->s.next == tail) {
					tmp->s.next = NULL;
					tail = tmp;
				}
				tmp = tmp->s.next;
			}
		}
		sbrk_ptr -= sizeof(union header) + header->s.size;
		return;
	}
	header->s.is_free = 1;
}

void*
realloc(void *block, size_t size)
{
	union header *header = ((union header *) block) -1;
	void *new_block;
  
	if(!block)
		return malloc(size);
	if(size <= header->s.size)
		return block;
	if(header == tail) {
		sbrk_ptr += size - header->s.size;
		header->s.size = size;
		return block;	
	}
	free(block);
	new_block = malloc(size);
	memcpy(block, new_block, header->s.size);
	return new_block;
}

void*
calloc(size_t nitems, size_t item_size) {
	void *tmp = malloc(nitems * item_size);
	memset(tmp, 0, nitems * item_size);
	return tmp;
}
