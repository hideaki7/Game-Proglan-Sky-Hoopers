.PHONY: build run clean

build:
	@if [ ! -f build/CMakeCache.txt ]; then \
		cd build && cmake .. ; \
	fi
	cd build && cmake --build .

run: build
	LIBGL_ALWAYS_SOFTWARE=1 ./build/doodlejump

clean:
	rm -rf build/*
