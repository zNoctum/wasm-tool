main.wasm: main.c
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
	wasm-opt --asyncify -Oz main.wasm -o main.wasm
	sh -c 'export BLOB=$$(cat main.wasm | base64 -w 0); envsubst < template.html > index.html'
	#uglifyjs opt.js -o opt.js

clean:
	rm main.wasm opt.js
