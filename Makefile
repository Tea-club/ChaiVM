BUILD_DIR := build
SRC_DIR := src
INC_DIR := include
TEST_DIR := test

JOBS ?= 4

.PHONY: all
all: test

.PHONY: format
.SILENT: format
format:
	./tools/clang-format.sh $(PWD)/$(SRC_DIR)
	./tools/clang-format.sh $(PWD)/$(INC_DIR)
	./tools/clang-format.sh $(PWD)/$(TEST_DIR)

.PHONY: build
build:
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR)
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: build-val
build-val:
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DBESM666_TEST_WITH_VALGRIND=ON
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: test
test: build
	ctest --test-dir $(PWD)/$(BUILD_DIR)/test --parallel $(JOBS) --output-on-failure

.PHONY: test-val
test-val: build-val
	ctest --test-dir $(PWD)/$(BUILD_DIR)/test --parallel $(JOBS) --output-on-failure

.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(PWD)/$(BUILD_DIR)
