index.html: out.js
	sh -c 'export JS_CODE=$$(cat out.js); envsubst < template.html > $@'

main.wasm: main.c libc/*
	clang main.c -O3 -nostdlib --target=wasm32 \
		-fvisibility=hidden \
		-flto \
		-Wl,--lto-O3 \
		-Wl,--no-entry \
		-Wl,--export-all \
		-Wl,--allow-undefined \
		-Wl,--import-memory \
		-Wl,--strip-all \
		-Wl,--export-dynamic \
		-o main.wasm
	wasm2wat main.wasm -o main.wat
	wasm-opt -O4 main.wasm -o main.wasm
	#wasm-opt --asyncify -Oz main.wasm -o main.wasm

out.js: template.js main.wasm
	sh -c 'export BLOB=$$(cat main.wasm | base64 -w 0); envsubst < template.js > $@'
clean:
	rm main.wa* opt.js index.html
