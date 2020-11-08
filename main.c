extern unsigned char __heap_base;
unsigned int bump_pointer = (unsigned int)&__heap_base;
#define WASM_EXPORT __attribute__((visibility("default")))
#define WASM_IMPORT(module, name) __attribute__((import_module(module), import_name(name))

void print(int);
void printc(char*);

WASM_EXPORT void* malloc(unsigned long size) {
	unsigned int ptr = bump_pointer;
	bump_pointer += size;
	return (void *)ptr;
}

void free(void* ptr) {  }

WASM_EXPORT int sum(int a[], int len) {
	int sum = 0;
	for(int i = 0; i < len; i++) {
		sum += a[i];
	}
	return sum;
}

WASM_EXPORT int main() {
	printc("test");
	return 0;
}
