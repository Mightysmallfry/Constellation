PRESET ?= default
BUILD_DIR := build

.PHONY: build test clean rebuild

build:
	cmake --preset $(PRESET)
	cmake --build $(BUILD_DIR)

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean build
