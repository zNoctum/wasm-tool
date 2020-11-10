#define WASM_EXPORT __attribute__((visibility("default")))
#define NULL 0

union header {
	struct {
		unsigned int size;
		unsigned char is_free;
		union header *next;
	} s;
	char align_stub[16];
};

extern unsigned char __heap_base;
unsigned int sbrk_ptr = (unsigned int)&__heap_base;
union header *head, *tail;

void*
malloc(unsigned long size)
{
	void *block;
	union header *header;
	if (!size)
		return NULL;
	header = head;
	while(header) {
		if (header->s.is_free && header->s.size >= size)
			return (void *)(header + 1);
		header = header->s.next;
	}
	block = (void *)sbrk_ptr;
	sbrk_ptr = sizeof(union header) + size;

	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
		head = header;
	if (tail)
		tail->s.next = header;
	tail = header;
	return (void*)(header + 1);
}

void free(void *block)
{
	union header *header, *tmp;
	void *programbreak;

	if(!block)
		return;
	header = (union header*)block - 1;

	if((unsigned int)block + header->s.size == sbrk_ptr) {
		if(head == tail) {
			head = tail = NULL;
		}
		else {
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
