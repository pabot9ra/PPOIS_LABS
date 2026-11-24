CMAKE ?= cmake
PYTHON ?= python3
BUILD_DIR ?= build
JOBS ?= 4

all:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug
	$(CMAKE) --build $(BUILD_DIR) --parallel $(JOBS)

test: all
	ctest --test-dir $(BUILD_DIR) --output-on-failure

run: all
	./$(BUILD_DIR)/postal

demo: all
	./$(BUILD_DIR)/postal --demo

coverage:
	$(CMAKE) -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DPOSTAL_COVERAGE=ON
	$(CMAKE) --build build-coverage --parallel $(JOBS)
	$(PYTHON) scripts/coverage.py build-coverage

clean:
	$(CMAKE) -E rm -rf build build-coverage

.PHONY: all test run demo coverage clean
