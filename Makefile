CONFIG?=Debug
BUILD_DIR?=build
GENERATOR_OPTION?=-G "Ninja"

# For Release build, set environment variable CONFIG.
# `export CONFIG=Release`
# To revert, run `unset CONFIG`


ifeq ($(OS), Windows_NT)
	# Windows
	STANDALONE_BIN_PATH=.\${BUILD_DIR}\src\Os251_artefacts\${CONFIG}\Standalone\OS-251.exe
	TESTS_BIN_PATH:=.\${BUILD_DIR}\tests\${CONFIG}\Os251_Tests.exe
	TESTS_USING_JUCE_BIN_PATH:=.\${BUILD_DIR}\tests/Os251_TestsUsingJuce_artefacts\${CONFIG}\Os251_TestsUsingJuce.exe
	BENCHMARK_BIN_PATH:=.\${BUILD_DIR}\benchmark\Os251_Benchmark_artefacts\${CONFIG}\Os251_Benchmark.exe
	RM_COMMAND=rmdir /s /q ${BUILD_DIR}
	# Use default just because I don't know how to use Ninja in Windows 
	GENERATOR_OPTION=
else
	UNAME_S:=$(shell uname -s)
	ifeq ($(UNAME_S), Darwin)
		# macOs
		STANDALONE_BIN_PATH=./$(BUILD_DIR)/src/Os251_artefacts/$(CONFIG)/Standalone/OS-251.app/Contents/MacOS/OS-251

	else
		# Linux
		STANDALONE_BIN_PATH:=./$(BUILD_DIR)/src/Os251_artefacts/$(CONFIG)/Standalone/OS-251

	endif
	TESTS_BIN_PATH:=./${BUILD_DIR}/tests/Os251_Tests
	TESTS_USING_JUCE_BIN_PATH:=./${BUILD_DIR}/tests/Os251_TestsUsingJuce_artefacts/${CONFIG}/Os251_TestsUsingJuce
	BENCHMARK_BIN_PATH:=./${BUILD_DIR}/benchmark/Os251_Benchmark_artefacts/${CONFIG}/Os251_Benchmark
	RM_COMMAND=rm -rf ${BUILD_DIR}
endif

#
# Build and run
#

# Configure the project and generate a native build system
configure:
	cmake . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(CONFIG) ${GENERATOR_OPTION}

build-all:
	cmake --build $(BUILD_DIR) --config $(CONFIG) --target Os251_All

# Build standalone
build-s:
	cmake --build $(BUILD_DIR) --config $(CONFIG) --target Os251_Standalone

# Run standalone
.PHONY: run-s
run-s: build-s
	$(STANDALONE_BIN_PATH)

.PHONY: lldb-s
lldb-s: build-s
	# Debug standalone with lldb
	lldb $(STANDALONE_BIN_PATH)

#
# Test
#

build-test:
	cmake --build $(BUILD_DIR) --config $(CONFIG) --target Os251_Tests

.PHONY: test
test: build-test
	${TESTS_BIN_PATH}

build-test-juce:
	cmake --build $(BUILD_DIR) --config $(CONFIG) --target Os251_TestsUsingJuce

.PHONY: test-juce
test-juce: build-test-juce
	${TESTS_USING_JUCE_BIN_PATH}

.PHONY: test-all
test-all: test test-juce
	make test
	make test-juce

#
# Benchmark
#

build-benchmark:
	cmake --build $(BUILD_DIR) --config $(CONFIG) --target Os251_Benchmark

.PHONY: benchmark
benchmark: build-benchmark
	${BENCHMARK_BIN_PATH}

#
# Lint
#

.PHONY: lint
lint:
	./lint.sh

.PHONY: lint-fix
lint-fix:
	./lint.sh fix

#
# All check
#
.PHONY: check-all
check-all:
	make lint
	make build-all
	make test
	make test-juce
	make benchmark

#
# Clean up
#

.PHONY: clean
clean:
	${RM_COMMAND}
