extern unsigned char __heap_base;
unsigned int bump_pointer = (unsigned int)&__heap_base;
#define EXPORT __attribute__((visibility("default")))

void draw_line(int, int, int, int);
void clear();
void sleep(unsigned int ms);

void print(int);
void printc(char*, int);

EXPORT void* malloc(unsigned long size) {
	unsigned int ptr = bump_pointer;
	bump_pointer += size;
	return (void *)ptr;
}

void free(void* ptr) {  }

EXPORT int sum(int a[], int len) {
	int sum = 0;
	for(int i = 0; i < len; i++) {
		sum += a[i];
	}
	return sum;
}

EXPORT int main() {
	while(1)
		for(int i = 0; i < 200; i++) {
			clear();
			draw_line(0, i, 200, i);
			sleep(100);
		}
	return 0;
}
