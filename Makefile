index.html: opt.js
	sh -c 'export JS_CODE=$$(cat opt.js); envsubst < template.html > $@'

main.wasm: main.c lib.c
	clang main.c -nostdlib --target=wasm32 \
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
	wasm-opt --asyncify -Oz main.wasm -o main.wasm

opt.js: template.js main.wasm
	sh -c 'export BLOB=$$(cat main.wasm | base64 -w 0); envsubst < template.js > $@'
	terser opt.js -c toplevel -o opt.js

clean:
	rm main.wasm opt.js index.html
