#include "lib.c"
#define WASM_EXPORT __attribute__((visibility("default")))

__attribute__((import_module("rawdraw"))) void rawdraw();

void print(int);
void printc(char*);

WASM_EXPORT int sum(int a[], int len) {
	int sum = 0;
	for(int i = 0; i < len; i++) {
		sum += a[i];
	}
	return sum;
}

WASM_EXPORT int main() {
	printc("test");
	rawdraw();
	return 0;
}
