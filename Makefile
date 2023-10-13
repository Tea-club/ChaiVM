BUILD_DIR := build
SRC_DIR := src
INC_DIR := include
TEST_DIR := test
BENCH_DIR := bench

JOBS ?= 4

.PHONY: all
all: test

.PHONY: init
init:
	git submodule init
	git submodule update

.PHONY: format
.SILENT: format
format:
	./tools/clang-format.sh $(PWD)/$(SRC_DIR)
	./tools/clang-format.sh $(PWD)/$(INC_DIR)
	./tools/clang-format.sh $(PWD)/$(TEST_DIR)
	./tools/clang-format.sh $(PWD)/$(BENCH_DIR)
.PHONY: build
build: init
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR)
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: build-val
build-val: init
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DBESM666_TEST_WITH_VALGRIND=ON
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: test
test: build
	export GTEST_COLOR=1 && ctest  --test-dir $(PWD)/$(BUILD_DIR)/test --parallel $(JOBS) --output-on-failure

.PHONY: test-val
test-val: build-val
	export GTEST_COLOR=1 && ctest --test-dir $(PWD)/$(BUILD_DIR)/test --parallel $(JOBS) --output-on-failure

.PHONY: bench
bench: init
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DCHAI_BENCH=1
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)
	cd $(PWD)/$(BUILD_DIR) && make run_bench && cd $(PWD)

.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(PWD)/$(BUILD_DIR)
