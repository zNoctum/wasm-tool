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

//-----------------------------------SYSCALL------------------------------------
extern unsigned char __heap_base;
unsigned long sbrk_ptr = (unsigned int)&__heap_base;

int
brk(unsigned long addr)
{
	sbrk_ptr = addr;
	return 0;
}

void*
sbrk(unsigned long inc)
{
	unsigned long tmp = sbrk_ptr;
	sbrk_ptr += inc;
	return (void *)tmp;
}
//------------------------------------------------------------------------------
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
	block = sbrk(sizeof(union header) + size);

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

	if((unsigned int)block + header->s.size == (unsigned int)sbrk(0)) {
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
		sbrk(-sizeof(union header) - header->s.size);
		return;
	}
	header->s.is_free = 1;
}
