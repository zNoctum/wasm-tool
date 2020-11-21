let blob = atob('${BLOB}');
var array = new Uint8Array(new ArrayBuffer(blob.length));
var i = 0|0;
for(i = 0; i < blob.length; i++) {
	array[i] = blob.charCodeAt(i);
}

var memory = new WebAssembly.Memory({initial:2});

var HEAPU8 = new Uint8Array(memory.buffer);

let dec = new TextDecoder("utf8");
function s(ptr) {
	var len;
	for(len = 0|0; HEAPU8[ptr+len] != 0|0; len++);
	return dec.decode(HEAPU8.subarray(ptr, ptr+len));
}

let wasmExports;

const obj = {
	env: {
		memory: memory,
		print: console.log,
		printc: (str) => { console.log(s(str)) },
	}
};

WebAssembly.instantiate(array, obj).then({instance} => {
	
	wasmExports = instance.exports;
	console.log(instance);
	instance.exports.main();
	
	const jsArray = [1, 2, 3, 4, 5];
	const cArrayPointer = instance.exports.malloc(jsArray.length * 4);
	const cArray = new Uint32Array(
		memory.buffer,
		cArrayPointer,
		jsArray.length
	);
	cArray.set(jsArray);
	console.log(instance.exports.sum(cArrayPointer, cArray.length));	
});
