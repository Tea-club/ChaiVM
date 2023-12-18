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
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DCHAIVM_ADD_MEM_SANITIZER=OFF -DCHAIVM_ADD_THREAD_SANITIZER=OFF
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)

.PHONY: test
test: build
	export GTEST_COLOR=1 && ctest  --test-dir $(PWD)/$(BUILD_DIR)/test --parallel $(JOBS) --output-on-failure

.PHONY: execute-tests-inside-make
execute-tests-inside-make:
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)
	export GTEST_COLOR=1 && ctest --test-dir $(PWD)/$(BUILD_DIR)/test --parallel $(JOBS) --output-on-failure


.PHONY: test-extended
test-extended: init
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DCHAIVM_ADD_MEM_SANITIZER=OFF -DCHAIVM_ADD_THREAD_SANITIZER=ON
	$(MAKE) execute-tests-inside-make
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DCHAIVM_ADD_MEM_SANITIZER=ON -DCHAIVM_ADD_THREAD_SANITIZER=OFF
	$(MAKE) execute-tests-inside-make

.PHONY: bench
bench: init
	cmake -S $(PWD) -B $(PWD)/$(BUILD_DIR) -DCHAI_BENCH=1 -DCMAKE_BUILD_TYPE=RELEASE
	cmake --build $(PWD)/$(BUILD_DIR) --parallel $(JOBS)
	cd $(PWD)/$(BUILD_DIR) && make run_bench && cd $(PWD)

.PHONY: clean
.SILENT: clean
clean:
	rm -rf $(PWD)/$(BUILD_DIR)
