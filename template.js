let blob = atob('${BLOB}');
var array = new Uint8Array(new ArrayBuffer(blob.length));
var i = 0|0;
for(i = 0; i < blob.length; i++) {
	array[i] = blob.charCodeAt(i);
}

var memory = new WebAssembly.Memory({initial:2});

var HEAP8 = new Int8Array(memory.buffer);
var HEAPU8 = new Uint8Array(memory.buffer);
var HEAP16 = new Int16Array(memory.buffer);
var HEAPU16 = new Uint16Array(memory.buffer);
var HEAP32 = new Uint32Array(memory.buffer);
var HEAPU32 = new Uint32Array(memory.buffer);
var HEAPF32 = new Float32Array(memory.buffer);
var HEAPF64 = new Float64Array(memory.buffer);

let dec = new TextDecoder("utf8");
function cstos(ptr) {
	var len;
	for(len = 0|0; HEAPU8[ptr+len] != 0|0; len++);
	return dec.decode(HEAPU8.subarray(ptr, ptr+len));
}

let wasmExports;

const obj = {
	env: {
		memory: memory,
		print: console.log,
		printc: (str) => { console.log(cstos(str)) },
	}
};

(async () => {
	let {instance} = await WebAssembly.instantiate(array, obj);
	
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
})();
